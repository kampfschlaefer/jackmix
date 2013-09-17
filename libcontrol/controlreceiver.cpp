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

#include "controlreceiver.h"
#include "controlsender.h"

namespace JackMix {
namespace MidiControl {

ControlReceiver::ControlReceiver(int parameter) {
	if (parameter >= 0)
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
