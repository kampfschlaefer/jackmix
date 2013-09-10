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
#include <exception>

#include <alsa/asoundlib.h>

namespace JackMix {
namespace MidiControl {

class MidiControlException : public std::exception {
private:
	const char* message;
public:
	MidiControlException(const char* msg = "General MIDI failure") { message = msg; }; 
	virtual const char* what() const throw() { return message; };
};

class ControlSender {
public:
	ControlSender(const char* port_name) throw (MidiControlException);
	~ControlSender();
protected:
	int port_id;
	snd_seq_t *seq_handle;
};


};
};

#endif
