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

#ifndef FLOATSLIDER_H
#define FLOATSLIDER_H

#include <qframe.h>
#include "common.h"

class QSlider;

class QFloatSlider : public QFrame {
Q_OBJECT
public:
	QFloatSlider( QWidget* =0, const char* =0 );
	QFloatSlider( JackMix::Direction, QWidget* p=0, const char* n=0 );
	QFloatSlider( float value, float min, float max, float pagestep, int precision, JackMix::Direction, QWidget* =0, const char* =0 );
	~QFloatSlider();
public slots:
	void setPrecision( int );
	void setPageStep( float );
	void setMinimum( float );
	void setMaximum( float );
	void setValue( float );
signals:
	void valueChanged( float );
private slots:
	void setValue( int );
private:
	void init( float value, float min, float max, float pagestep, int precision, JackMix::Direction );
	void setMinMax();
	QSlider* _slider;
	float _value, _min, _max, _pagestep;
	int _precision;
	JackMix::Direction _dir;
};

#endif

