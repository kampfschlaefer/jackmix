#include <QtCore/QString>

#include <exception>
#include <alsa/asoundlib.h>

#include "controlsender.h"

namespace JackMix {
namespace MidiControl {
	
ControlSender::ControlSender(const char* port_name) throw (MidiControlException) {
	if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_DUPLEX, 0) < 0)
		throw new MidiControlException("Can't connect to MIDI subsystem");
	
	// Name the client
	snd_seq_set_client_name(seq_handle, port_name);

	// open one input port
	if ((port_id = snd_seq_create_simple_port(seq_handle, "Control Input",
	                                         SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
	                                         SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
		throw new MidiControlException("Can't open MIDI port");
	
}

ControlSender::~ControlSender() {
	snd_seq_delete_simple_port(seq_handle, port_id);
	snd_seq_close(seq_handle);
}

};
};
