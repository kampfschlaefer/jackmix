#ifndef CONTROLSENDER_TEST_H
#define CONTROLSENDER_TEST_H

#include "../libgui/slider.h"
#include "controlreceiver.h"

class ControlledSlider : public JackMix::GUI::Slider, JackMix::MidiControl::ControlReceiver {
Q_OBJECT
public:
	ControlledSlider(int channel, double value, double min, double max, int precision, double pagestep, QWidget* p);
	~ControlledSlider();
};

#endif
