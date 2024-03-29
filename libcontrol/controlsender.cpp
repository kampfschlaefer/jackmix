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

#include <QtCore/QtDebug>
#include <QtCore/QString>

#include <unistd.h>
#include <alsa/asoundlib.h>
#include <exception>

#include "controlsender.h"
#include "controlsender.moc"
#include "controlreceiver.h"

namespace JackMix {
namespace MidiControl {
	
PortListener::PortListener(snd_seq_t *handle, int id) :
	port_id(id), seq_handle(handle)
{

}

void PortListener::run()
{
	int npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
	
	// Using C style memory allocation for communication with the kernel-level MIDI system
	struct pollfd *pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
	snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);
	snd_seq_event_t *ev;
	int param;
	int value;
	
	running = true;
	
	while (running) {
		// *ix system calls are uninterruptible, so we'll pop back into
		// user-space every 200ms or so
		if (poll(pfd, npfd, 200) > 0) {
			do {
				// Returning -ENOSPC means the MIDI buffer overran, so check for an error here
				if (snd_seq_event_input(seq_handle, &ev) >= 0) {
					if (ev -> type == SND_SEQ_EVENT_CONTROLLER) {
						param = ev->data.control.param;
						value = ev->data.control.value;
						emit message(param, value);
					}
					snd_seq_free_event(ev);
				}
			} while (snd_seq_event_input_pending(seq_handle, 0) > 0);
		}
		
		//qDebug() << "Tick";

	}
	
	exit(0);
}

ControlSender::ControlSender(const char* port_name) 
	: have_alsa_seq {false}
	, port_listener {nullptr}
{

	if ((have_alsa_seq = snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_DUPLEX, 0)) == 0) {
		// Name the client
		snd_seq_set_client_name(seq_handle, port_name);

		// Open one input port
		if ((port_id = snd_seq_create_simple_port(seq_handle, "Control Input",
							SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
							SND_SEQ_PORT_TYPE_APPLICATION)) < 0)
			throw new MidiControlException("Can't open MIDI port");
		
		port_listener = new JackMix::MidiControl::PortListener(seq_handle, port_id);
		port_listener->start();
		connect (port_listener, SIGNAL(message(int, int)), this, SLOT(despatch_message(int, int)) );
	} else
		seq_handle = nullptr;
}

ControlSender::~ControlSender() {
	// close down the port listener thread
	port_listener->quit();
	port_listener->wait();
	// then close the MIDI port
	if (have_alsa_seq) {
		snd_seq_delete_simple_port(seq_handle, port_id);
		snd_seq_close(seq_handle);
	}
}

QList<ControlReceiver*> ControlSender::dtab[maxMidiParam];

void ControlSender::subscribe(ControlReceiver *receiver, int parameter) {
	if (parameter >= 0 && parameter < maxMidiParam) {
		unsubscribe(receiver, parameter);
		dtab[parameter].append(receiver);
	}
}

void ControlSender::unsubscribe(ControlReceiver *receiver) {
	// remove receiver from all routing table entries
	for (int i = 0; i < maxMidiParam; i++)
		unsubscribe(receiver, i);
}
void ControlSender::unsubscribe(ControlReceiver *receiver, int parameter) {
	if (parameter > 0 && parameter < maxMidiParam)  // remove all occurrences from this routing entry
		dtab[parameter].removeAll(receiver);
}

void ControlSender::despatch_message(int param, int val) {
	// Ignore parameter index past end of the dispatch table.
	// That includes the "specials" like MIDI all-off, which, it turns out,
	// is one of the reserved CC messages referred to as Channel Control messages
	// (controller # >= 122) See for example:
	//	https://cmtext.indiana.edu/MIDI/chapter3_channel_mode_messages.php
	if (param < maxMidiParam) {
		QListIterator<ControlReceiver*> iterator(dtab[param]);
		while (iterator.hasNext())
			iterator.next()->controlEvent(param, val);
	}
}

};
};
