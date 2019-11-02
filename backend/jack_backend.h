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

#ifndef JACKMIX_JACK_BACKEND_H
#define JACKMIX_JACK_BACKEND_H

#include <QtCore/QtMath>
#include <QtCore/QMap>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTime>
#include <QException>
#include <jack/jack.h>
#include <jack/midiport.h>


#include "backend_interface.h"

class QDomElement;
class QDomDocument;

namespace JackMix {

int process( ::jack_nframes_t, void* );

typedef QMap<QString,jack_port_t*> portsmap;
typedef portsmap::Iterator ports_it;
typedef portsmap::ConstIterator ports_cit;

class JackBackend : public BackendInterface {

	friend int process( ::jack_nframes_t, void* );
public:
	/// Initializes the connection
	JackBackend( GuiServer_Interface* );
	/// Ends everything
	~JackBackend();

	bool addOutput( QString );
	bool removeInput( QString );
	bool addInput( QString );
	bool removeOutput( QString );
	/**
	 * Rename a jack port
	 * 
	 * \param old_name current port name
	 * \param new_name new port name
	 * \returns whether a port was renamed
	 */
	bool rename(const QString old_name, const QString new_name);
	bool rename(const QString old_name, const char *new_name);
	bool renameInput(const QString old_name, const QString new_name);
	bool renameInput(const QString old_name, const char *new_name);
	bool renameOutput(const QString old_name, const QString new_name);
	bool renameOutput(const QString old_name, const char *new_name);
	
private:
	FaderState& getMatrixVolume(QString, QString);
	void setOutVolume( QString, float );
	FaderState& getOutVolume( QString );
	void setInVolume( QString, float );
	FaderState& getInVolume( QString );
	
	bool rename(portsmap &map, QStringList &lst, const QString old_name, const char *new_name);
	
public:
	/// sets the volume of channel,output
	void setVolume( QString,QString,float );
	/// returns the volume of channel,output
	float getVolume(QString in, QString out) {
		return getMatrixVolume(in, out).target;
	}
	
	/// returns a QStringList with the names of the out-channels
	const QStringList &outchannels() const { return out_ports_list; };
	/// returns a QStringList with the names of the in-channels
	const QStringList &inchannels() const { return in_ports_list; };

private:
	/// As their name suggests, the following are QMaps of the input
	/// and output ports onto a jack_client_t*...
	portsmap in_ports;
	portsmap out_ports;
	/// ...but we must also keep a list of names of the channels, so
	/// the order is preserved. This is vitally important when saving
	/// the layout in a file, for example.
	QStringList in_ports_list;
	QStringList out_ports_list;
	
	::jack_client_t *client;
	/// First dimension is input-channels, second is output-channels
	QMap<QString,QMap<QString,FaderState> > volumes;
	QHash<QString,FaderState> outvolumes;
	QHash<QString,FaderState> involumes;
	
	/// Process the second and third bytes of the MIDI CC message
	/// (the JACK process() routine can't do this because it's not a QObject
	void send_signal(const ::jack_midi_data_t b1, const ::jack_midi_data_t b2);
	/// Port to listen on for MIDI signals
	::jack_port_t *midi_port;
	
	/**
	 * Handle changes in jack server sample rate
	 * 
	 * @param rate New sample rate
	 * @param args Client-supplied args (not used)
	 * @returns 0 (success)
	 */
	::JackSampleRateCallback new_srate(::jack_nframes_t rate, void* args) {
		set_interp_len(rate);
		return 0;
	}
};

class InvalidMatrixFaderException : public QException {
public:
	InvalidMatrixFaderException(QString const& message)
	: message {message}
	{ }
	
	InvalidMatrixFaderException* clone() const {
		return new InvalidMatrixFaderException(*this);
	}
	
	void raise() const { throw *this; }
	
	QString getMessage() const { return message; }

private:
	QString message;
};

};

#endif
