/* Copyright */

#ifndef JACKMIX_JACK_BACKEND_H
#define JACKMIX_JACK_BACKEND_H

//#include <vector>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <jack/jack.h>

namespace JackMix {

typedef QMap<QString,jack_port_t*> portsmap;
typedef portsmap::Iterator ports_it;

class JackBackend {
public:
	static JackBackend* backend() { static JackBackend* backend = new JackBackend(); return backend; }

	void addOutput( QString );
	void addInput( QString );

	/// sets the volume of channel,output
	void setVolume( QString,QString,float );
	/// returns the volume of channel,output
	float getVolume( QString,QString );

	void setOutVolume( QString, float );
	float getOutVolume( QString );
	void setInVolume( QString, float );
	float getInVolume( QString );

	/// returns a QStringList with the names of the out-channels
	QStringList outchannels();
	/// returns a QStringList with the names of the in-channels
	QStringList inchannels();
private:
	/// Initializes the connection
	JackBackend();
	/// Ends everything
	~JackBackend();

public:
	portsmap in_ports;
	portsmap out_ports;
	::jack_client_t *client;
	/// First dimension is input-channels, second is output-channels
	QMap<QString,QMap<QString,float> > volumes;
	QMap<QString,float> outvolumes;
	QMap<QString,float> involumes;
};

#define BACKEND JackMix::JackBackend::backend()

int process( ::jack_nframes_t, void* );

};

#endif
