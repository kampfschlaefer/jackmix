/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

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
//#include "jack_backend.moc"

#include <iostream>
#include <qdom.h>

using namespace JackMix;

JackBackend::JackBackend() {
	std::cout << "JackBackend::JackBackend()" << std::endl;
	client = ::jack_client_new( "JackMix" );
	//client = 0;
	if ( client ) {
		::jack_set_process_callback( client, JackMix::process, this );
		std::cout << "JackBackend::JackBackend() activate" << std::endl;
		::jack_activate( client );
	}
	else
	std::cout << "\n No jack-connection! :(\n\n";
	std::cout << "JackBackend::JackBackend() finished" << std::endl;
}
JackBackend::~JackBackend() {
	std::cout << "JackBackend::~JackBackend()" << std::endl;
	if ( client )
		::jack_client_close( client );
}

bool JackBackend::addOutput( QString name ) {
	if ( client ) {
		out_ports.insert( name, jack_port_register ( client, name.latin1(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 ) );
		return true;
	}
	return false;
}
bool JackBackend::addInput( QString name ) {
	if ( client ) {
		in_ports.insert( name, jack_port_register ( client, name.latin1(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 ) );
		return true;
	}
	return false;
}

bool JackBackend::removeOutput( QString name ) {
	if ( client )
		jack_port_unregister( client, out_ports[ name ] );
	out_ports.remove( name );
	return true;
}
bool JackBackend::removeInput( QString name ) {
	if ( in_ports.find( name ) != in_ports.end() ) {
		if ( client )
			jack_port_unregister( client, in_ports[ name ] );
		in_ports.remove( name );
		return true;
	}
	return false;
}

void JackBackend::setVolume( QString channel, QString output, float volume ) {
	//std::cerr << "JackBackend::setVolume( " << channel << ", " << output << ", " << volume << " )" << std::endl;
	if ( channel == output ) {
		if ( involumes.contains( channel ) )
			setInVolume( channel, volume );
		else
			setOutVolume( channel, volume );
	} else
		volumes[ channel ][ output ] = volume;
}

float JackBackend::getVolume( QString channel, QString output ) {
	//std::cerr << "JackBackend::getVolume( " << channel << ", " << output << " ) = " << volumes[ channel ][ output ] << std::endl;
	volumes[ channel ].insert( output, 0, FALSE );
	return volumes[ channel ][ output ];
}

void JackBackend::setOutVolume( QString ch, float n ) {
	//std::cerr << "JackBackend::setOutVolume(QString " << ch << ", float " << n << " )" << std::endl;
	outvolumes[ ch ] = n;
}
float JackBackend::getOutVolume( QString ch ) {
	//std::cerr << "JackBackend::getOutVolume(QString " << ch << " )" << std::endl;
	outvolumes.insert( ch, 1, FALSE );
	return outvolumes[ ch ];
}
void JackBackend::setInVolume( QString ch, float n ) {
	//std::cerr << "JackBackend::setInVolume(QString " << ch << ", float " << n << " )" << std::endl;
	involumes[ ch ] = n;
}
float JackBackend::getInVolume( QString ch ) {
	//std::cerr << "JackBackend::getInVolume(QString " << ch << " )" << std::endl;
	involumes.insert( ch, 1, FALSE );
	return involumes[ ch ];
}

QStringList JackBackend::outchannels() {
	QStringList tmp;
	JackMix::ports_it it;
	for ( it = out_ports.begin(); it != out_ports.end(); ++it )
		tmp << it.key();
	return tmp;
}
QStringList JackBackend::inchannels() {
	QStringList tmp;
	JackMix::ports_it it;
	for ( it = in_ports.begin(); it != in_ports.end(); ++it )
		tmp << it.key();
	return tmp;
}

void JackBackend::toXML( QDomDocument doc, QDomElement elem ) {
std::cout << "JackBackend::toXML()" << std::endl;
	QDomElement matrix = doc.createElement( "matrix" );

	QStringList ins = inchannels();
	QStringList outs = outchannels();
	for ( uint i=0; i<ins.size(); i++ ) {
		QDomElement in = doc.createElement( "instrip" );
		in.setAttribute( "channel", ins[ i ] );
		for ( uint j=0; j<outs.size(); j++ ) {
			QDomElement out = doc.createElement( "out" );
			out.setAttribute( "channel", outs[ j ] );
			out.setAttribute( "value", getVolume( ins[ i ], outs[ j ] ) );
			in.appendChild( out );
		}
		matrix.appendChild( in );
	}

	elem.appendChild( matrix );
}
void JackBackend::fromXML( QDomElement elem ) {
	QString inch;
	if ( elem.tagName() == "matrix" )
		for ( QDomNode n = elem.firstChild(); !n.isNull(); n=n.nextSibling() ) {
			QDomElement tmp = n.toElement();
			if ( !tmp.isNull() ) {
				if ( tmp.tagName() == "instrip" ) {
					inch = tmp.attribute( "channel" );
					for ( QDomNode m = tmp.firstChild(); !m.isNull(); m=m.nextSibling() ) {
						QDomElement tmp2 = m.toElement();
						if ( !tmp2.isNull() ) {
							QString outch( tmp2.attribute( "channel" ) );
							float value = tmp2.attribute( "value", "0" ).toFloat();
							setVolume( inch, outch, value );
						}
					}
				}
			}
		}
}


int JackMix::process( jack_nframes_t nframes, void* arg ) {
	//std::cout << "JackMix::process( jack_nframes_t " << nframes << ", void* )" << std::endl;
	JackMix::JackBackend* backend = static_cast<JackMix::JackBackend*>( arg );
	QMap<QString,jack_default_audio_sample_t*> ins;
	JackMix::ports_it it;
	for ( it = backend->in_ports.begin(); it!=backend->in_ports.end(); ++it )
		ins.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.data(), nframes ) );
	QMap<QString,jack_default_audio_sample_t*> outs;
	for ( it = backend->out_ports.begin(); it != backend->out_ports.end(); ++it )
		outs.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.data(), nframes ) );
	ports_it in_it;
	ports_it out_it;
	/// Blank outports...
	for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
		jack_default_audio_sample_t* tmp = outs[ out_it.key() ];
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] = 0;
	}
	/// Adjust inlevels.
	for ( in_it = backend->in_ports.begin(); in_it != backend->in_ports.end(); ++in_it ) {
		jack_default_audio_sample_t* tmp = ins[ in_it.key() ];
		float volume = backend->getInVolume( in_it.key() );
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] *= volume;
	}
	/// The actual mixing.
	for ( in_it = backend->in_ports.begin(); in_it != backend->in_ports.end(); ++in_it ) {
		jack_default_audio_sample_t* tmpin = ins[ in_it.key() ];
		for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
			jack_default_audio_sample_t* tmpout = outs[ out_it.key() ];
			float tmpvolume = backend->getVolume( in_it.key(), out_it.key() );
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				tmpout[ n ] += tmpin[ n ] * tmpvolume;
			}
		}
	}
	/// Adjust outlevels.
	for ( out_it = backend->out_ports.begin(); out_it != backend->out_ports.end(); ++out_it ) {
		jack_default_audio_sample_t* tmp = outs[ out_it.key() ];
		float volume = backend->getOutVolume( out_it.key() );
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] *= volume;
	}
	return 0;
}

