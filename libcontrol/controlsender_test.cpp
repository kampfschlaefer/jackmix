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

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

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

	ControlledSlider *slider = new ControlledSlider(7, 0, 0, 128, 4, 8, 0);
	slider->show();

	int ret = qapp->exec();
	return ret;
}
