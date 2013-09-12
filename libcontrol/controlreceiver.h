#ifndef CONTROLRECEIVER_H
#define CONTROLRECEIVER_H

namespace JackMix {
namespace MidiControl {

class ControlReceiver {
public:
	ControlReceiver(int parameter = -1);
	~ControlReceiver();
	virtual void controlEvent(int param, int value) =0;
};

};
};

#endif
