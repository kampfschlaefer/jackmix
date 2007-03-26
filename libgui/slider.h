/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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

#ifndef SLIDER_H
#define SLIDER_H

#include "abstractslider.h"

namespace JackMix {
namespace GUI {

class Slider : public AbstractSlider
{
Q_OBJECT
public:
	Slider( double value, double min, double max, int precision, double pagestep, QWidget*, QString = "%1 dB" );
	~Slider();

	void value( double );

protected:
	void paintEvent( QPaintEvent* );

	void mouseEvent( QMouseEvent* );
	QRect _faderarea;

private slots:
	void timeout();

private:
	QTimer* _timer;
	bool _show_value;
};

}; // GUI
}; //JackMix

#endif // SLIDER_H

