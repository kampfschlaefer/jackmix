#include <QtGui/QApplication>
#include <QtGui/QWidget>

#include "controlsender.h"
#include "../libgui/slider.h"

int main(int argc, char *argv[])
{
	QApplication *qapp = new QApplication( argc, argv );

	JackMix::MidiControl::ControlSender cs("Test Connection");

	JackMix::GUI::Slider *mw = new JackMix::GUI::Slider(0, 0, 128, 4, 32, 0);
	QObject::connect (&cs, SIGNAL(controlSignal(int)), mw, SLOT(setMidiValue(int)) );
	mw->show();

	int ret = qapp->exec();
	return ret;
}
