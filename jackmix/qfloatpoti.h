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

#ifndef QFLOATPOTI_H
#define QFLOATPOTI_H

#include <qframe.h>

class QFloatPoti_private;

class QFloatPoti : public QFrame {
Q_OBJECT
public:
	QFloatPoti( QWidget*, const char* =0 );
	~QFloatPoti();
public slots:
	void setValue( float );
	void setColor( QColor );
protected:
	void paintEvent( QPaintEvent* );
	void wheelEvent( QWheelEvent* );
private:
	QFloatPoti_private* d;
};

#endif

