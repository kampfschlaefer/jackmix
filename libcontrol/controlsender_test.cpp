#include <QtGui//QApplication>
#include <QtGui/QWidget>
#include <QtGui/QLabel>

#include "controlsender.h"


int main(int argc, char *argv[])
{
	QApplication *qapp = new QApplication( argc, argv );

	JackMix::MidiControl::ControlSender cs("Test Connection");

	QWidget *mw = new QLabel("A MIDI app called \"Test Connection\" should have appeared", 0);
	mw->show();

	int ret = qapp->exec();
	return ret;
}
