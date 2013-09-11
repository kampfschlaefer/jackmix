#ifndef CONTROLSENDER_H
#define CONTROLSENDER_H

/** ControlSender Create and Service incomming MIDI control sequences
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
#include <exception>

#include <alsa/asoundlib.h>

namespace JackMix {
namespace MidiControl {

class PortListener;

/** MidiControlException: thrown when there are problems communicating with
 *  the kerenel's MIDI subsystem
 */
class MidiControlException : public std::exception {
private:
	const char* message;
public:
	MidiControlException(const char* msg = "General MIDI failure") { message = msg; }; 
	virtual const char* what() const throw() { return message; };
};

/** ControlSender: Open a MIDI port and route control messages to zero or more recipients */
class ControlSender : public QObject {
Q_OBJECT
public:
	/** Listen on a new virtual port with the given port name */
	ControlSender(const char* port_name) throw (MidiControlException);
	~ControlSender();

signals:
	void controlSignal(int val); //<! broadcast any change of control value

protected:
	int port_id;
	snd_seq_t *seq_handle;
	
protected slots:
	void despatch_message(int ch, int val); //<! Route message

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
	/** emit incomming MIDI messages
	 * @param param  Parameter of the incomming control message
	 * @param val Value of the incomming message
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
