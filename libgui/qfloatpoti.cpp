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

//#include "qpoti.h"
#include <qsynthKnob.h>
#include <QtGui/QLayout>
#include <iostream>

class QFloatPoti_private {
public:
	QFloatPoti_private() : poti( 0 ), min( 0 ), max( 1 ), value( 0.5 ), precision( 100 ), inupdate( false ), layout( 0 ) {
	}
	//QPoti* poti;
	qsynthKnob* poti;
	float min, max, value;
	int precision;
	bool inupdate;

	void setMinMax() {
		poti->setRange( int( min*precision ), int( max*precision ) );
	}

	QBoxLayout* layout;
};

QFloatPoti::QFloatPoti( float value, float min, float max, int precision, QColor color, QWidget* p, const char* n )
 : QFrame( p )
 , d( new QFloatPoti_private() )
{
	d->layout = new QVBoxLayout( this );
	d->layout->setMargin( 0 );
	d->layout->setSpacing( 0 );
	d->poti = new qsynthKnob( this );
	d->poti->setRange( 0, 100 );
	d->poti->setSingleStep( 4 );
	connect( d->poti, SIGNAL( valueChanged( int ) ), this, SLOT( iValueChanged( int ) ) );
	d->layout->addWidget( d->poti, 0, Qt::AlignCenter );
	setPrecision( precision );
	setMinimum( min );
	setMaximum( max );
	setValue( value );
	setColor( color );
	//d->poti->setText( n );
	//qDebug( "QFloatPoti::QFloatPoti : text=%s", d->poti->text().ascii() );
	//d->poti->setLabel( false );
}

QFloatPoti::QFloatPoti( QWidget* p, const char* n )
 : QFrame( p )
 , d( new QFloatPoti_private() )
{
	d->layout = new QVBoxLayout( this );
	d->layout->setMargin( 0 );
	d->layout->setSpacing( 0 );
	d->poti = new qsynthKnob( this );
	d->poti->setRange( 0, 100 );
	d->poti->setSingleStep( 4 );
	connect( d->poti, SIGNAL( valueChanged( int ) ), this, SLOT( iValueChanged( int ) ) );
	d->layout->addWidget( d->poti );
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
	d->poti->setKnobColor( n );
}

void QFloatPoti::iValueChanged( int n ) {
	d->value = n / float( d->precision );
	emit valueChanged( d->value );
}

