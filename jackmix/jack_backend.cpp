
#include "jack_backend.h"
//#include "jack_backend.moc"

#include <iostream>

using namespace JackMix;

JackBackend::JackBackend() {
	std::cout << "JackBackend::JackBackend()" << std::endl;
	client = ::jack_client_new( "JackMix" );
	::jack_set_process_callback( client, JackMix::process, 0 );
	std::cout << "JackBackend::JackBackend() ports" << std::endl;
	addOutput( "out_L" );
	addOutput( "out_R" );
	std::cout << "JackBackend::JackBackend() activate" << std::endl;
	::jack_activate( client );
	std::cout << "JackBackend::JackBackend() finished" << std::endl;
}
JackBackend::~JackBackend() {
	std::cout << "JackBackend::~JackBackend()" << std::endl;
	::jack_client_close( client );
}

void JackBackend::addOutput( QString name ) {
	out_ports.insert( name, jack_port_register ( client, name.latin1(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 ) );
}
void JackBackend::addInput( QString name ) {
	in_ports.insert( name, jack_port_register ( client, name.latin1(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 ) );
}

void JackBackend::setVolume( QString channel, QString output, float volume ) {
	volumes[ channel ][ output ] = volume;
}

float JackBackend::getVolume( QString channel, QString output ) {
	volumes[ channel ].insert( output, 1, FALSE );
	return volumes[ channel ][ output ];
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

int JackMix::process( jack_nframes_t nframes, void* /*arg*/ ) {
//	std::cout << "JackMix::process( jack_nframes_t " << nframes << ", void* )" << std::endl;
	QMap<QString,jack_default_audio_sample_t*> ins;
	JackMix::ports_it it;
	for ( it = BACKEND->in_ports.begin(); it!=BACKEND->in_ports.end(); ++it )
		ins.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.data(), nframes ) );
	QMap<QString,jack_default_audio_sample_t*> outs;
	for ( it = BACKEND->out_ports.begin(); it != BACKEND->out_ports.end(); ++it )
		outs.insert( it.key(), (jack_default_audio_sample_t*) jack_port_get_buffer( it.data(), nframes ) );
	ports_it in_it;
	ports_it out_it;
//	std::cout << "JackMix::process 1" << std::endl;
	for ( out_it = BACKEND->out_ports.begin(); out_it != BACKEND->out_ports.end(); ++out_it ) {
		jack_default_audio_sample_t* tmp = outs[ out_it.key() ];
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] = 0;
	}
//	std::cout << "JackMix::process 2" << std::endl;
	for ( in_it = BACKEND->in_ports.begin(); in_it != BACKEND->in_ports.end(); ++in_it ) {
		jack_default_audio_sample_t* tmpin = ins[ in_it.key() ];
		for ( out_it = BACKEND->out_ports.begin(); out_it != BACKEND->out_ports.end(); ++out_it ) {
			jack_default_audio_sample_t* tmpout = outs[ out_it.key() ];
			float tmpvolume = BACKEND->getVolume(  in_it.key(), out_it.key() );
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				tmpout[ n ] += tmpin[ n ] * tmpvolume;
			}
		}
	}
//	std::cout << "JackMix::process 3" << std::endl;
	for ( out_it = BACKEND->out_ports.begin(); out_it != BACKEND->out_ports.end(); ++out_it ) {
		jack_default_audio_sample_t* tmp = outs[ out_it.key() ];
		for ( jack_nframes_t n=0; n<nframes; n++ ) tmp[ n ] /= 2;
	}
//	std::cout << "JackMix::process 4" << std::endl;
	return 0;
}

