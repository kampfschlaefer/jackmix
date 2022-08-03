/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

    Modified to permit control via MIDI by Nick Bailey <nick@n-ism.org>
    Released as version 0.5.0 (et seq.) copyright 2013.
    
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

#ifndef AUX_ELEMENTS_H
#define AUX_ELEMENTS_H

#include <QtCore/QVector>

#include <mixingmatrix.h>
#include <dbvolcalc.h>
#include <controlreceiver.h>
#include <QRadioButton>
namespace JackMix {

namespace GUI {
	class Knob;
	class MidiControlChannelAssigner;
}

namespace MixerElements {
	
/**
 * Simpliest form of a control connecting one input with one output.
 */
class AuxElement : public JackMix::MixingMatrix::Element
                 , public dB2VolCalc {
Q_OBJECT
public:
	AuxElement( QStringList inchannel, QStringList outchannel, MixingMatrix::Widget*, const char* =0 );
	~AuxElement();

	int inchannels() const { return 1; }
	int outchannels() const { return 1; }
	
	void setIndicator(const QColor& c);
 
public slots:
	void emitvalue( double );

private:
	JackMix::GUI::Knob *_poti;
	QRadioButton *_button;
};

void init_aux_elements();

}; // MixerElements
}; // JackMix

#endif // AUX_ELEMENTS_H

