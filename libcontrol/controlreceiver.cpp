#include <QtCore/QtDebug>

#include "controlreceiver.h"
#include "controlsender.h"

namespace JackMix {
namespace MidiControl {

ControlReceiver::ControlReceiver(int parameter) {
	ControlSender::subscribe(this, parameter);
}

ControlReceiver::~ControlReceiver() {
	ControlSender::unsubscribe(this);
}

void ControlReceiver::controlEvent(int param, int value) {
	//qDebug() << "ControlReceiver::controlEvent: parameter " << param << ", value " << value;
}


};
};
