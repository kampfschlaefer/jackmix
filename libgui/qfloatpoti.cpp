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

#include "qfloatpoti.h"
#include "qfloatpoti.moc"

#include "qpoti.h"
#include <QtGui/QLayout>
#include <iostream>

class QFloatPoti_private {
public:
	QFloatPoti_private() : poti( 0 ), min( 0 ), max( 1 ), value( 0.5 ), precision( 100 ), inupdate( false ) {
	}
	QPoti* poti;
	float min, max, value;
	int precision;
	bool inupdate;

	void setMinMax() {
		poti->setRange( int( min*precision ), int( max*precision ) );
	}
};

QFloatPoti::QFloatPoti( float value, float min, float max, int precision, QColor color, QWidget* p, const char* n )
 : QFrame( p )
 , d( new QFloatPoti_private() )
{
	QBoxLayout* _layout = new QVBoxLayout( this );
	d->poti = new QPoti( 0,100,1,0, this );
	connect( d->poti, SIGNAL( valueChanged( int ) ), this, SLOT( iValueChanged( int ) ) );
	_layout->addWidget( d->poti );
	setPrecision( precision );
	setMinimum( min );
	setMaximum( max );
	setValue( value );
	setColor( color );
	d->poti->setText( n );
	//qDebug( "QFloatPoti::QFloatPoti : text=%s", d->poti->text().ascii() );
	d->poti->setLabel( false );
}

QFloatPoti::QFloatPoti( QWidget* p, const char* n )
 : QFrame( p )
 , d( new QFloatPoti_private() )
{
	QBoxLayout* _layout = new QVBoxLayout( this );
	d->poti = new QPoti( 0,100,1,0, this );
	connect( d->poti, SIGNAL( valueChanged( int ) ), this, SLOT( iValueChanged( int ) ) );
	_layout->addWidget( d->poti );
}
QFloatPoti::~QFloatPoti() {
}

void QFloatPoti::setPrecision( int n ) {
	d->precision = n;
}
void QFloatPoti::setPageStep( float ) {
}
void QFloatPoti::setMinimum( float n ) {
	d->min = n;
	d->setMinMax();
}
void QFloatPoti::setMaximum( float n ) {
	d->max = n;
	d->setMinMax();
}

void QFloatPoti::setValue( float n ) {
	if ( !d->inupdate ) {
		d->inupdate = true;
		if ( n > d->max )
			d->value = d->max;
		else
			if ( n < d->min )
				d->value = d->min;
			else
				d->value = n;
		d->poti->setValue( int( d->value * d->precision ) );
		d->inupdate = false;
	}
}

void QFloatPoti::setColor( QColor n ) {
	d->poti->setColor( n );
}

void QFloatPoti::iValueChanged( int n ) {
	d->value = n / float( d->precision );
	emit valueChanged( d->value );
}

