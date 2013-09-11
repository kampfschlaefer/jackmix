#include <QtGui/QApplication>
#include <QtGui/QWidget>

#include "controlsender.h"
#include "../libgui/slider.h"
#include "controlsender_test.h"
#include "controlsender_test.moc"

ControlledSlider::ControlledSlider(int channel, double value, double min, double max, int precision, double pagestep, QWidget* p) :
	Slider(value, min, max, precision, pagestep, p),
	ControlReceiver(channel)
{
}

ControlledSlider::~ControlledSlider()
{
}

void ControlledSlider::controlEvent(int param, int value) {
	setMidiValue(value);
}


int main(int argc, char *argv[])
{
	QApplication *qapp = new QApplication( argc, argv );

	JackMix::MidiControl::ControlSender cs("Test Connection");

	ControlledSlider *slider = new ControlledSlider(6, 0, 0, 128, 4, 8, 0);
	QObject::connect (&cs, SIGNAL(controlSignal(int)), slider, SLOT(setMidiValue(int)) );
	slider->show();

	int ret = qapp->exec();
	return ret;
}
