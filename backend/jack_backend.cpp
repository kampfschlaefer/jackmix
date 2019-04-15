/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>
 
    Copyright 2013 Nick Bailey <nick@n-ism.org>
    
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#include "jack_backend.h"
#include <jack/midiport.h>

//#include "jack_backend.moc"

#include <QtCore/QDebug>

using namespace JackMix;

JackBackend::JackBackend( GuiServer_Interface* g ) : BackendInterface( g ) {
	//qDebug() << "JackBackend::JackBackend()";
	client = ::jack_client_open( "JackMix", JackNoStartServer, NULL );
	if ( client ) {
		midi_port = ::jack_port_register(client, "Control",
		                                 JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
		::jack_set_process_callback( client, JackMix::process, this );
		//qDebug() << "JackBackend::JackBackend() activate";
		::jack_activate( client );
	}
	else {
		qWarning() << "\n No jack-connection! :(\n\n";
		gui->message( "No Jack-connection :-(", "<qt><p>Sorry, I couldn't connect to Jack. This probably means that <b>no jackd is running</b>. Please start it and try JackMix again.</p><p>If you don't know what I am talking about, than JackMix might not be the program you want...</p></qt>" );
	}
	//qDebug() << "JackBackend::JackBackend() finished";
}
JackBackend::~JackBackend() {
	//qDebug() << "JackBackend::~JackBackend()";
	if ( client ) {
		/*qDebug() << " return code" <<*/ ::jack_deactivate( client );
		/*qDebug() << " return code" <<*/ ::jack_client_close( client );
	}
}

bool JackBackend::addOutput( QString name ) {
	if ( client ) {
		//qDebug() << "JackBackend::addOutput(" << name << ")";
		out_ports.insert( name, jack_port_register ( client, name.toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 ) );
		out_ports_list << name;
		return true;
	}
	return false;
}
bool JackBackend::addInput( QString name ) {
	if ( client ) {
		//qDebug() << "JackBackend::addInput(" << name << ")";
		in_ports.insert( name, jack_port_register ( client, name.toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 ) );
		in_ports_list << name;
		return true;
	}
	return false;
}

bool JackBackend::removeOutput( QString name ) {
	//qDebug() << "JackBackend::removeOutput(" << name << ")";
	if ( client && out_ports.find( name ) != out_ports.end() )
		jack_port_unregister( client, out_ports[ name ] );
	out_ports.remove( name );
	out_ports_list.removeOne( name );
	return true;
}
bool JackBackend::removeInput( QString name ) {
	//qDebug() << "JackBackend::removeInput(" << name << ")";
	if ( client && in_ports.find( name ) != in_ports.end() )
		jack_port_unregister( client, in_ports[ name ] );
	in_ports.remove( name );
	in_ports_list.removeOne( name );
	return true;
}
bool JackBackend::rename(const QString old_name, const QString new_name) {
	return rename(old_name, new_name.toStdString().c_str());
}
bool JackBackend::rename(const QString old_name, const char *new_name) {
	return renameInput(old_name, new_name) || renameOutput(old_name, new_name);
}
bool JackBackend::renameInput(const QString old_name, const QString new_name) {

	// Rename the port
	bool done_it = renameInput(old_name, new_name.toStdString().c_str());
	
	if (done_it) { // Maintain the volume maps
		if (involumes.contains(old_name)) {
			involumes.insert(new_name, involumes[old_name]);
			involumes_new.insert( new_name, involumes_new[ old_name ] );
			involumes.remove(old_name);
			involumes_new.remove(old_name);
		}
		if (volumes.contains(old_name)) {
			volumes.insert(new_name, volumes[old_name]);
			volumes_new.insert( new_name, volumes[ old_name ] );
			volumes.remove(old_name);
			volumes_new.remove( old_name );
		}
	}
	
	return done_it;
}
bool JackBackend::renameInput(const QString old_name, const char *new_name) {
	return rename(in_ports, in_ports_list, old_name, new_name);
}
bool JackBackend::renameOutput(const QString old_name, const QString new_name) {
	bool done_it = renameOutput(old_name, new_name.toStdString().c_str());
	
	if (done_it) {
		if (outvolumes.contains(old_name)) {
			outvolumes.insert(new_name, outvolumes[old_name]);
			outvolumes_new.insert( new_name, outvolumes[ old_name ] );
			outvolumes.remove(old_name);
			outvolumes_new.remove( old_name );
		}
		QStringListIterator ipi(in_ports_list);
		while(ipi.hasNext()) {
			QString input(ipi.next());
			if (volumes[input].contains(old_name)) {
				volumes[input].insert(new_name, volumes[input][old_name]);
				volumes_new[ input ].insert( new_name, volumes[ input ][ old_name ] );
				volumes[input].remove(old_name);
				volumes_new[ input ].remove( old_name );
			}
		}
	}
	
	return done_it;
}
bool JackBackend::renameOutput(const QString old_name, const char *new_name) {
	return rename(out_ports, out_ports_list, old_name, new_name);
}
bool JackBackend::rename(portsmap &map, QStringList &lst, const QString old_name, const char *new_name) {
	bool done_it = false;
	
	if (client) {
		ports_cit port_i;
		// Check to see if the name is in this map
		if ((port_i=map.constFind(old_name)) != map.constEnd()) {
			// Found it... try to rename the jack port
			jack_port_t *port = port_i.value();
			done_it = (jack_port_rename(client, port, new_name) == 0);
			// If that works, go on to update our port map
			if (done_it) {
				map.insert(new_name, port);
				map.remove(old_name);
				// We have to maintain the port lists as well as the maps
				int pos;
				if ((pos = lst.indexOf(old_name)) >= 0)
					lst[pos] = new_name;
				//qDebug()<<"JackBackend::rename -- list now"<<lst;
			}			
		}
	}
	
	return done_it;
}


void JackBackend::updateVolume( QString channel, QString output, float volume ) {
	//qDebug() << "JackBackend::updateVolume( " << channel << ", " << output << ", " << volume << " )";
	if ( channel == output ) {
		if ( involumes.contains( channel ) )
			setInVolume( channel, volume );
		else
			setOutVolume( channel, volume );
	} else
		volumes[ channel ][ output ] = volume;
}

void JackBackend::setVolume(QString channel, QString output, float volume) {
	//qDebug() << "JackBackend::setVolume( " << channel << ", " << output << ", " << volume << " )";
	if ( channel == output ) {
		if ( involumes_new.contains( channel ) )
			setInVolumeNew( channel, volume );
		else
			setOutVolumeNew( channel, volume );
	} else
		volumes_new[ channel ][ output ] = volume;
}

float JackBackend::getVolume( QString channel, QString output ) {
	//qDebug() << "JackBackend::getVolume( " << channel << ", " << output << " ) = " << volumes[ channel ][ output ];
	if ( channel == output ) {
		if ( outvolumes.contains( channel ) )
			return getOutVolume( channel );
		if ( involumes.contains( channel ) )
			return getInVolume( channel );
	} else {
		//if ( !volumes.contains( channel ) )
		//	volumes.insert( channel, QMap<QString,float> );
		if ( !volumes[ channel ].contains( output ) )
			volumes[ channel ].insert( output, 0 );
		return volumes[ channel ][ output ];
	}
	return 0;
}

float JackBackend::getVolumeNew(QString channel, QString output) {
	//qDebug() << "JackBackend::getVolumeNew( " << channel << ", " << output << " ) = " << volumes[ channel ][ output ];
	if ( channel == output ) {
		if ( outvolumes_new.contains( channel ) )
			return getOutVolumeNew( channel );
		if ( involumes_new.contains( channel ) )
			return getInVolumeNew( channel );
	} else {
		//if ( !volumes.contains( channel ) )
		//	volumes.insert( channel, QMap<QString,float> );
		if ( !volumes_new[ channel ].contains( output ) ) {
			if ( volumes[ channel ].contains( output ) ) {
				volumes_new[ channel ].insert( output, volumes[ channel ][ output ]);
			} else {
				volumes_new[ channel ].insert( output, 0 );
			}
		}
		return volumes_new[ channel ][ output ];
	}
	return 0;
}

void JackBackend::setOutVolume( QString ch, float n ) {
	//qDebug() << "JackBackend::setOutVolume(QString " << ch << ", float " << n << " )";
	outvolumes[ ch ] = n;
}

void JackBackend::setOutVolumeNew( QString ch, float n ) {
	//qDebug() << "JackBackend::setOutVolumeNew(QString " << ch << ", float " << n << " )";
	outvolumes_new[ ch ] = n;
}

float JackBackend::getOutVolume( QString ch ) {
	//qDebug() << "JackBackend::getOutVolume(QString " << ch << " )";
	//outvolumes.insert( ch, 1 );
	if ( !outvolumes.contains( ch ) )
		outvolumes.insert( ch, 1 );
	return outvolumes[ ch ];
}

float JackBackend::getOutVolumeNew( QString ch ) {
	//qDebug() << "JackBackend::getOutVolumeNew(QString " << ch << " )";
	if ( !outvolumes_new.contains( ch ) )
		outvolumes_new.insert( ch, 1 );
	return outvolumes_new[ ch ];
}

void JackBackend::setInVolume( QString ch, float n ) {
	//qDebug() << "JackBackend::setInVolume(QString " << ch << ", float " << n << " )";
	involumes[ ch ] = n;
}

void JackBackend::setInVolumeNew(QString ch , float n) {
	//qDebug() << "JackBackend::setInVolumeNew(QString " << ch << ", float " << n << " )";
	involumes_new[ ch ] = n;
}

float JackBackend::getInVolume( QString ch ) {
	//qDebug() << "JackBackend::getInVolume(QString " << ch << " )";
	//involumes.insert( ch, 1 );
	if ( !involumes.contains( ch ) )
		involumes.insert( ch, 1 );
	return involumes[ ch ];
}

float JackBackend::getInVolumeNew(QString ch) {
	//qDebug() << "JackBackend::getInVolumeNew(QString " << ch << " )";
	//involumes.insert( ch, 1 );
	if ( !involumes_new.contains( ch ) ) {
		if ( involumes.contains( ch ) ) {
			involumes_new.insert( ch, involumes[ ch ] );
		} else {
			involumes_new.insert( ch, 1 );
		}
	}
	return involumes_new[ ch ];
}

void JackBackend::send_signal(const ::jack_midi_data_t b1,
			      const ::jack_midi_data_t b2)  {
	emit(cc_message(b1, b2));
}


int JackMix::process( jack_nframes_t nframes, void* arg ) {
	//qDebug() << "JackMix::process( jack_nframes_t " << nframes << ", void* )";
	JackMix::JackBackend* backend = static_cast<JackMix::JackBackend*>( arg );

	// Deal with MIDI events
	void* midi_buffer { ::jack_port_get_buffer(backend->midi_port, nframes) };
	::jack_midi_event_t event;
	::jack_nframes_t event_count { jack_midi_get_event_count(midi_buffer) };
	
	for (::jack_nframes_t e {0}; e < event_count; e++) {
		::jack_midi_event_get(&event, midi_buffer, e);
		if (event.buffer[0] == 0xb0) // It's a control change message
			backend->send_signal(event.buffer[1], event.buffer[2]);
	}
	
	QMap<QString,jack_default_audio_sample_t*> ins;
	JackMix::ports_it it;
	for ( it = backend->in_ports.begin(); it!=backend->in_ports.end(); ++it )
		ins.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.value(), nframes ) );
	QMap<QString,jack_default_audio_sample_t*> outs;
	for ( it = backend->out_ports.begin(); it != backend->out_ports.end(); ++it )
		outs.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.value(), nframes ) );
	ports_it in_it;
	ports_it out_it;
	/// Blank outports...
	for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
		jack_default_audio_sample_t* tmp = outs[ out_it.key() ];
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] = 0;
	}
	/// Adjust inlevels.
	for ( in_it = backend->in_ports.begin(); in_it != backend->in_ports.end(); ++in_it ) {
		QString key {in_it.key()};
		jack_default_audio_sample_t* tmp = ins[ key ];
		float volume = backend->getInVolume( key );
		float volume_new = backend->getInVolumeNew( key );
		float max {0};
		if ( !qFuzzyCompare(volume_new, volume)) {
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				tmp[ n ] *=	volume + n * ( volume_new - volume ) / nframes;
				max = qMax(max, tmp[n]);
			}
		} else {
			for ( jack_nframes_t n=0; n<nframes; n++) {
				tmp[ n ] *= volume_new;
				max = qMax(max, tmp[n]);
			}
		}
		backend->newInputLevel( key, max );
		if ( volume_new != volume ) {
			backend->setInVolume( key, volume_new );
		}
	}
		 
	/// The actual mixing.
	for ( in_it = backend->in_ports.begin(); in_it != backend->in_ports.end(); ++in_it ) {
		jack_default_audio_sample_t* tmpin = ins[ in_it.key() ];
		for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
			jack_default_audio_sample_t* tmpout = outs[ out_it.key() ];
			float volume = backend->getVolume( in_it.key(), out_it.key() );
			float volume_new = backend->getVolumeNew( in_it.key(), out_it.key() );
			if (!qFuzzyCompare(volume_new, volume)) {
				for ( jack_nframes_t n=0; n<nframes; ++n ) {
					tmpout[ n ] += tmpin[ n ] * ( volume + n * (volume_new - volume) / nframes );
				}
			} else {
				for ( jack_nframes_t n=0; n<nframes; n++ ) {
					tmpout[ n ] += tmpin[ n ] * volume_new;
				}
			}
			if ( volume != volume_new) {
				backend->updateVolume( in_it.key(), out_it.key(), volume_new);
			}
		}
	}
	/// Adjust outlevels.
	for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
		QString key {out_it.key()};
		jack_default_audio_sample_t* tmp = outs[ key ];
		float volume = backend->getOutVolume( key );
		float volume_new = backend->getOutVolumeNew( key );
		float max {0};
		if ( !qFuzzyCompare(volume_new, volume)) {
			qDebug() << "BIG CHANGE: " << volume_new << " != " << volume;
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				tmp[ n ] *=	volume + n * ( volume_new - volume ) / nframes;
				max = qMax( max, static_cast<float>( tmp[ n ] ) );
			}
		} else {
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				tmp[ n ] *= volume;
				max = qMax( max, static_cast<float>( tmp[ n ] ) );
			}
		}
		backend->newOutputLevel(key, max);
		if ( volume_new != volume ) {
			backend->setOutVolume( key, volume_new );
		}
	}

	// Send any information about channel levels
	backend->report();
        
	return 0;
}

