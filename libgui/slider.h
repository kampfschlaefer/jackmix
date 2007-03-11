/*
    Copyright ( C ) 2006 Arnold Krille <arnold@arnoldarts.de>

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

#include <QtGui/QWidget>
#include "dbvolcalc.h"

namespace JackMix {
namespace GUI {

class Slider : public QWidget, public dB2VolCalc
{
Q_OBJECT
public:
	Slider( float value, float min, float max, int precision, float pagestep, QWidget*, QString = "%1 dB" );
	~Slider();

	QSizePolicy sizePolicy() const { return QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ); }

	void value( float );
	float value() const { return _value; }
signals:
	void valueChanged( float );

protected:
	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void wheelEvent( QWheelEvent* );

private:
	void mouseEvent( QMouseEvent* );
	float _value, _pagestep;
	bool _value_inupdate;
	int _precision;
	QString _valuestring;
	QRect _faderarea;
};

}; // GUI
}; //JackMix

#endif // SLIDER_H

