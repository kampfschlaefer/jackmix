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

#include <qpainter.h>

QFloatPoti::QFloatPoti( QWidget* p, const char* n )
 : QFrame( p,n )
 , _min( 0 )
 , _max( 1 )
 , _value( 0 )
{
	this->setMinimumSize( 30,30 );
}
QFloatPoti::~QFloatPoti() {
}

void QFloatPoti::setValue( float n ) {
	if ( n > _max )
		_value = _max;
	else
		if ( n < _min )
			_value = _min;
		else
			_value = n;
}

void QFloatPoti::paintEvent( QPaintEvent* ) {
	QPainter *painter = new QPainter( this );
	int w = rect().width()-30;
	int h = rect().height()-30;
	if ( w > h ) w = h;
		else h = w;

	/// Paint the actual value as text
	painter->drawText( 10,20, QString::number( _value ) );
	/// Paint the circle
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->drawEllipse( -w/2,-h/2,w,h );

	/// Paint tickmarks
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	painter->drawLine( w/2,0, w/2 + 10,0 );
	painter->drawText( w/2 + 10, 0, QString::number( 0 ) );
	for ( float i=0.1; i<=1.01; i+=0.1 ) {
		painter->rotate( 280/10 );
		painter->drawLine( w/2,0, w/2 + 10,0 );
		painter->drawText( w/2 + 10, 0, QString::number( i ) );
	}

	/// Paint tickmarks
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	painter->drawLine( w/2,0, w/2 + 10,0 );
	for ( float i=0.05; i<=1.01; i+=0.05 ) {
		painter->rotate( 280/20 );
		painter->drawLine( w/2,0, w/2 + 5,0 );
	}

	/// Paint a bar vor the value
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	painter->rotate( 280*_value );
	painter->drawLine( 0,0, w/2,0 );

	delete painter;
}

void QFloatPoti::wheelEvent( QWheelEvent* ev ) {
	float step = 0.05;
	if ( ev->delta() > 0 ) {
		setValue( _value + step );
	} else {
		setValue( _value - step );
	}
	repaint();
}

