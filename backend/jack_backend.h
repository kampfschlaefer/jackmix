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

#ifndef JACKMIX_JACK_BACKEND_H
#define JACKMIX_JACK_BACKEND_H

#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>
#include <jack/jack.h>

class QDomElement;
class QDomDocument;

namespace JackMix {

typedef QMap<QString,jack_port_t*> portsmap;
typedef portsmap::Iterator ports_it;

class JackBackend {
public:
	/// Initializes the connection
	JackBackend();
	/// Ends everything
	~JackBackend();

	void addOutput( QString );
	void removeInput( QString );
	void addInput( QString );
	void removeOutput( QString );

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

	void toXML( QDomDocument, QDomElement );
	void fromXML( QDomElement );
private:

public:
	portsmap in_ports;
	portsmap out_ports;
	::jack_client_t *client;
	/// First dimension is input-channels, second is output-channels
	QMap<QString,QMap<QString,float> > volumes;
	QMap<QString,float> outvolumes;
	QMap<QString,float> involumes;
};

int process( ::jack_nframes_t, void* );

};

#endif
