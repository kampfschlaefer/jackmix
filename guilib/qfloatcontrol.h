/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

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

#ifndef QFLOATCONTROL_H
#define QFLOATCONTROL_H

#include <qframe.h>

namespace JackMix {
namespace GUI {

class QFloatControl : public QFrame
{
Q_OBJECT
Q_PROPERTY( float min READ min WRITE min )
Q_PROPERTY( float max READ max WRITE max )
public:
	QFloatControl( QWidget*, const char* =0 );
	~QFloatControl();

	void paintEvent( QPaintEvent* );

	void min( float ) {}
	void max( float ) {}
	void value( float ) {}
	float min() const { return _min; }
	float max() const { return _max; }
	float value() const { return _value; }
private:
	float _min, _max, _value;
};

}; // GUI
}; // JackMix

#endif // QFLOATCONTROL_H

