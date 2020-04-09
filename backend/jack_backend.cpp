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
		set_interp_len(::jack_get_sample_rate(client));
	}
	else {
		qWarning() << "\n No jack-connection! :(\n\n";
		gui->message( "No Jack-connection :-(", "<qt><p>Sorry, I couldn't connect to Jack. This probably means that <b>no jackd is running</b>. Please start it and try JackMix again.</p><p>If you don't know what I am talking about, then JackMix might not be the program you want...</p></qt>" );
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
		/*
		 * We need to register inputs by creating entries
                 * in the involumes hash if the interpolation functions
                 * are to be used. Just referencing them is sufficient
		 * to create a partially initialised FaderState.
		 * 
		 * setOutput() will finish the job when the output level
		 * is set.
		 */
		outvolumes[name];
		
		out_ports.insert( name, jack_port_register ( client, name.toStdString().c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 ) );
		out_ports_list << name;
		return true;
	}
	return false;
}
bool JackBackend::addInput( QString name ) {
	if ( client ) {
		//qDebug() << "JackBackend::addInput(" << name << ")";
		
		/* Register the input port (see above) */
		involumes[name];
		
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
			involumes.remove(old_name);
		}
		if (volumes.contains(old_name)) {
			volumes.insert(new_name, volumes[old_name]);
			volumes.remove(old_name);
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
			outvolumes.remove(old_name);
		}
		QStringListIterator ipi(in_ports_list);
		while(ipi.hasNext()) {
			QString input(ipi.next());
			if (volumes[input].contains(old_name)) {
				volumes[input].insert(new_name, volumes[input][old_name]);
				volumes[input].remove(old_name);
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


void JackBackend::setVolume(QString channel, QString output, float volume) {
	//qDebug() << "JackBackend::setVolume( " << channel << ", " << output << ", " << volume << " )";
	if ( channel == output ) {
		// addInput or addOutput will have created uninitialised FaderState entries,
                // so we can use that in the following test.
		if ( involumes.contains( channel ) )
			setInVolume(channel, volume);
		else
			setOutVolume(channel, volume);
	} else {
 		if (!volumes[channel].contains(output)) {
			//qDebug() << "Creating new FaderState. interp_len = " << interp_len;
 			volumes[channel].insert(output, FaderState(volume, this));
		} else
			volumes[channel][output] = volume;
        }
}

JackBackend::FaderState& JackBackend::getMatrixVolume( QString channel, QString output ) {
	//qDebug() << "JackBackend::getVolume(" << channel << ", " << output << ");
	static JackBackend::FaderState invalid(-1, nullptr); // no likee - somebody might change it. FIXME
	
	if ( channel == output ) {
		if ( outvolumes.contains( channel ) )
			return getOutVolume(channel);
		if ( involumes.contains( channel ) )
			return getInVolume(channel);
	} else {
		if (!volumes[channel].contains(output) ){//|| volumes[channel][output].p == nullptr) {
                        qDebug() << "Inserting new FaderState in volumes.";
			volumes[channel].insert(output, FaderState(0, this));
		}

		return volumes[channel][output];
	}
	
	// Should never get here.
	throw(InvalidMatrixFaderException(channel + "->" + output));
	// Consequently, this won't happen...
	return invalid;
}

void JackBackend::setOutVolume( QString ch, float n ) {
	//qDebug() << "JackBackend::setOutVolume(QString " << ch << ", float " << n << " )";
	
	if (outvolumes[ch].p == nullptr) { // need to construct a new Faderstate
		//qDebug() << "New output FaderState, volume = " << n;
		outvolumes.insert(ch, FaderState(n, this));
	} else {                           // update the existing one
		outvolumes[ch] = n;
	}
}

JackBackend::FaderState& JackBackend::getOutVolume( QString ch ) {
	return outvolumes[ch];
}

void JackBackend::setInVolume( QString ch, float n ) {
	//qDebug() << "JackBackend::setInVolume(QString " << ch << ", float " << n << " )";
	
	if (involumes[ch].p == nullptr) { // need to construct a new Faderstate
		//qDebug() << "New input FaderState, volume = " << n;
		involumes.insert(ch, FaderState(n, this));
	} else {                          // update the existing one
		involumes[ch] = n;
	}
}

JackBackend::FaderState&  JackBackend::getInVolume( QString ch ) {
	//qDebug() << "JackBackend::getInVolume(QString " << ch << " )";
	return involumes[ch];
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

		float max =
			backend->interp_fader<jack_default_audio_sample_t>(
				tmp, nframes, backend->getInVolume(key)
			);
		
		backend->newInputLevel(key, max);
	}
		 
	/// The actual mixing.
	for ( in_it = backend->in_ports.begin(); in_it != backend->in_ports.end(); ++in_it ) {
		jack_default_audio_sample_t* tmpin = ins[ in_it.key() ];
		for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
			jack_default_audio_sample_t* tmpout = outs[ out_it.key() ];
			backend->interp_fader<jack_default_audio_sample_t>(
				tmpout, tmpin, nframes, backend->getMatrixVolume(in_it.key(), out_it.key())
			);
 		}
		
	}
	/// Adjust outlevels.
	for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
		QString key {out_it.key()};
 		jack_default_audio_sample_t* tmp = outs[ key ];
		
		float max =
			backend->interp_fader<jack_default_audio_sample_t>(
				tmp, nframes, backend->getOutVolume(key)
			);
		
		backend->newOutputLevel(key, max);
	}

	// Send any information about channel levels
	backend->report();
        
	return 0;
}

