namespace JackMix {
namespace MidiControl {

class ControlReceiver {
public:
	ControlReceiver(int parameter);
	~ControlReceiver();
	virtual void controlEvent(int param, int value) =0;
};

};
};
