/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library. If not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#ifndef KNOB_H
#define KNOB_H

#include "abstractslider.h"

class QTimer;

namespace JackMix {
namespace GUI {

class Knob : public AbstractSlider
{
Q_OBJECT
public:
	Knob( double value, double min, double max, int precision, double pagestep, QWidget*, QString = "%1 dB" );
	~Knob();

	void value( double );

protected:
	void paintEvent( QPaintEvent* );

private slots:
	void timeOut();

private:
	void mouseEvent( QMouseEvent* );

	QTimer *_timer;
	bool _show_value;
};

}; // GUI
}; // JackMix
#endif // KNOB_H

