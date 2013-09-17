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
