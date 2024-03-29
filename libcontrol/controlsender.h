/*
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

#ifndef CONTROLSENDER_H
#define CONTROLSENDER_H

/** ControlSender Create and Service incoming MIDI control sequences
 * 
 * An application should have one instance of this class for each MIDI port
 * which is requires monitoring (so jackmix has only one). The lifecycle
 * of a ControlSender class is as follows:
 * 
 * 1. Open a named port (which will appear on the MIDI patch bay
 *    of routing applications like QJackCtl)
 * 
 * 2. Start a thread which listens to this port in the background
 *    and forwards control values to clients which have registered
 *    an interest in them
 * 
 * 3. Accept registration requests from clients to be notified of
 *    MIDI control change events or disconnected from events
 * 
 * Finally stop the listening thread and close the port.
 */

#include <QtCore/QString>
#include <QtCore/QThread>
#include <QtCore/QList>

#include <exception>

#include <alsa/asoundlib.h>

namespace JackMix {
namespace MidiControl {

class PortListener;
class ControlReceiver;

/** MidiControlException: thrown when there are problems communicating with
 *  the kerenel's MIDI subsystem
 */
class MidiControlException : public std::exception {
private:
	const char* message;
public:
	MidiControlException(const char* msg = "General MIDI failure") { message = msg; }; 
	virtual const char* what() noexcept { return message; };
};

/** ControlSender: Open a MIDI port and route control messages to zero or more recipients */
class ControlSender : public QObject {
Q_OBJECT
public:
	/** Listen on a new virtual port with the given port name */
	ControlSender(const char* port_name);
	~ControlSender();
	/** Have certain controls signals sent to a particular receiver (but only once!) */
	static void subscribe(ControlReceiver *receiver, int parameter);
	/** Remove all reistration of a particular receiver for all parameter changes
	 * @param receiver The ControlReceiver to deregister
	 */
	static void unsubscribe(ControlReceiver *receiver);
	/** Remove all registration of a particular receiver for a given parameter change
	 * @param receiver  The ControlReceiver to deregister
	 * @param parameter Remove only from a given control parameter
	 */
	static void unsubscribe(ControlReceiver *receiver, int parameter);

signals:
	void controlSignal(int val); //<! broadcast any change of control value

protected:
        /** ALSA sequencer available and opened successfully */
        bool have_alsa_seq;

protected:
	int port_id;
	snd_seq_t *seq_handle;
	static constexpr int maxMidiParam = 120;
	/** Dispatch table
	 *  An array of vectors, one for each MIDI parameter read from the input
	 *  channel.
	 */
	static QList<ControlReceiver *> dtab[maxMidiParam];

public slots:
	void despatch_message(int ch, int val);      //<! Route message

private:
	PortListener *port_listener;
};

/** PortListener implements a QThread which monitors the MIDI port
 *  allocated by the creating class. A PortListener instance
 *  emits the channel and value of all MIDI control messages
 *  which arrive on that port.
 */
class PortListener : public QThread {
Q_OBJECT
public:
	PortListener(snd_seq_t *handle, int id);
	~PortListener() { };
	/** Let the listener terminate at its earliest convenience */
	void quit(void) { running = false; };

signals:
	/** emit incoming MIDI messages
	 * @param param  Parameter of the incoming control message
	 * @param val Value of the incoming message
	 */
	void message(int param, int val);

protected:
	virtual void run();
	bool running;

private:
	int port_id;
	snd_seq_t *seq_handle;

};

};
};

#endif
