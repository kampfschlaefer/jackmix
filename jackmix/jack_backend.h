/* Copyright */

#ifndef JACKMIX_JACK_BACKEND_H
#define JACKMIX_JACK_BACKEND_H

#include <vector>
#include <jack/jack.h>

namespace JackMix {

class JackBackend {
public:
	static JackBackend* backend() { static JackBackend* backend = new JackBackend(); return backend; }

	void addOutput( const char* );
	void addInput( const char* );

	/// sets the volume of channel,output
	void setVolume( int,int,float );
	/// returns the volume of channel,output
	float getVolume( int,int );
private:
	/// Initializes the connection
	JackBackend();
	/// Ends everything
	~JackBackend();

public:
	std::vector <jack_port_t*> in_ports;
	std::vector <jack_port_t*> out_ports;
	::jack_client_t *client;
	/// First dimension is input-channels, second is output-channels
	std::vector<std::vector<float> > volumes;
};

int process( ::jack_nframes_t, void* );

};

#endif
