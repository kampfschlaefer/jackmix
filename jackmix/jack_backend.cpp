
#include "jack_backend.h"
//#include "jack_backend.moc"

#include <iostream>

using namespace JackMix;

JackBackend::JackBackend() {
	std::cout << "JackBackend::JackBackend()" << std::endl;
	client = ::jack_client_new( "JackMix" );
	::jack_set_process_callback( client, JackMix::process, 0 );
//	std::cout << "JackBackend::JackBackend() ports" << std::endl;
//	addOutput( "out_L" );
//	addOutput( "out_R" );
//	addInput( "in_L" );
//	addInput( "in_R" );
	std::cout << "JackBackend::JackBackend() activate" << std::endl;
	::jack_activate( client );
	std::cout << "JackBackend::JackBackend() finished" << std::endl;
}
JackBackend::~JackBackend() {
	std::cout << "JackBackend::~JackBackend()" << std::endl;
	::jack_client_close( client );
}

void JackBackend::addOutput( const char* name ) {
	out_ports.push_back( jack_port_register ( client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0 ) );
	for ( unsigned int i=0; i<in_ports.size(); i++ ) {
		volumes[ i ].push_back( 1 );
	}
}
void JackBackend::addInput( const char* name ) {
	in_ports.push_back( jack_port_register ( client, name, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0 ) );
	volumes.push_back( std::vector<float>( out_ports.size(), 1 ) );
}

void JackBackend::setVolume( int channel, int output, float volume ) {
	volumes[ channel ][ output ] = volume;
}

float JackBackend::getVolume( int channel, int output ) {
	return volumes[ channel ][ output ];
}


int JackMix::process( jack_nframes_t nframes, void* /*arg*/ ) {
//	std::cout << "JackMix::process( jack_nframes_t " << nframes << ", void* )" << std::endl;
	std::vector<jack_default_audio_sample_t*> ins;
	for ( unsigned int i=0; i<JackMix::JackBackend::backend()->in_ports.size(); i++ )
		ins.push_back( (jack_default_audio_sample_t*) jack_port_get_buffer( JackMix::JackBackend::backend()->in_ports[ i ], nframes ) );
	std::vector<jack_default_audio_sample_t*> outs;
	for ( unsigned int i=0; i<JackMix::JackBackend::backend()->out_ports.size(); i++ )
		outs.push_back( (jack_default_audio_sample_t*) jack_port_get_buffer( JackMix::JackBackend::backend()->out_ports[ i ], nframes ) );
//	jack_default_audio_sample_t *in1 = ( jack_default_audio_sample_t* ) jack_port_get_buffer ( JackMix::JackBackend::backend()->in_ports.at( 0 ), nframes );
//	jack_default_audio_sample_t *in2 = ( jack_default_audio_sample_t* ) jack_port_get_buffer ( JackMix::JackBackend::backend()->in_ports.at( 1 ), nframes );
//	jack_default_audio_sample_t *out1 = ( jack_default_audio_sample_t* ) jack_port_get_buffer ( JackMix::JackBackend::backend()->out_ports.at( 0 ), nframes );
//	jack_default_audio_sample_t *out2 = ( jack_default_audio_sample_t* ) jack_port_get_buffer ( JackMix::JackBackend::backend()->out_ports.at( 1 ), nframes );
	for ( unsigned int i=0; i<outs.size(); i++ ) {
		for ( unsigned int j=0; j<ins.size(); j++ ) {
			for ( jack_nframes_t n=0; n<nframes; n++ ) {
				outs[ i ][ n ] = ins[ j ][ n ] * JackMix::JackBackend::backend()->volumes[ j ][ i ];
			}
		}
		for ( jack_nframes_t n=0; n<nframes; n++ )
			outs[ i ][ n ] /= ins.size();
	}
	return 0;
}

