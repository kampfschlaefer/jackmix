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
#include <qvaluevector.h>

class QFloatPoti_private {
public:
	QFloatPoti_private() : min( 0 ), max( 1 ), value( 0 ), color( QColor( 255,50,50 ) ) {
	}
	float min, max, value;
	QColor color;
};

struct TickmarkLabel {
	QPoint point;
	QString label;
	TickmarkLabel( QPoint p, QString l ) : point( p ), label( l ) {}
	TickmarkLabel() {}
};

QFloatPoti::QFloatPoti( QWidget* p, const char* n )
 : QFrame( p,n )
 , d( new QFloatPoti_private() )
{
	this->setMinimumSize( 30,30 );
}
QFloatPoti::~QFloatPoti() {
}

void QFloatPoti::setValue( float n ) {
	if ( n > d->max )
		d->value = d->max;
	else
		if ( n < d->min )
			d->value = d->min;
		else
			d->value = n;
}

void QFloatPoti::setColor( QColor n ) {
	d->color = n;
	repaint();
}

void QFloatPoti::paintEvent( QPaintEvent* ) {
	QPainter *painter = new QPainter( this );
	int w = rect().width();
	int h = rect().height();
	if ( w > h ) w = h;
		else h = w;
	/// Length of tickmarks not longer than 15px
	int tickmarkwidth = int( h * 0.1 );
	if ( tickmarkwidth > 15 )
		tickmarkwidth = 15;
	w -= 3*tickmarkwidth;
	h -= 3*tickmarkwidth;

	/// Paint the circle
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->setBrush( d->color );
	painter->drawEllipse( -w/2,-h/2,w,h );

	/// Points for the tickmark-texts
	QValueVector<TickmarkLabel> points;

	/// Paint tickmarks
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	painter->drawLine( w/2,0, w/2 + tickmarkwidth,0 );
	points.push_back( TickmarkLabel( painter->xForm( QPoint( w/2 + tickmarkwidth,0 ) ), "0" ) );
	for ( float i=0.1; i<=1.01; i+=0.1 ) {
		painter->rotate( 280/10 );
		painter->drawLine( w/2,0, w/2 + tickmarkwidth,0 );
		points.push_back( TickmarkLabel( painter->xForm( QPoint( w/2 + tickmarkwidth,0 ) ), QString::number( i ) ) );
	}

	/// Paint the labels around the tickmarks
/*	painter->resetXForm();
	for ( uint i=0; i<points.size(); i++ ) {
		painter->drawText( QRect( points[ i ].point, QSize( 20, 10 ) ), Qt::AlignCenter, points[ i ].label );
	}*/

	/// Paint small tickmarks
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	for ( float i=0.05; i<=1.01; i+=0.05 ) {
		painter->rotate( 280/20 );
		painter->drawLine( w/2,0, w/2 + tickmarkwidth/2,0 );
	}

	/// Paint a bar vor the value
	painter->resetXForm();
	painter->translate( rect().width()/2, rect().height()/2 );
	painter->rotate( -230 );
	painter->rotate( 280*d->value );
	painter->setPen( QPen( QColor( 0,0,0 ), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin ) );
	painter->drawLine( 0,0, w/2,0 );

	/// Paint the actual value as text
	painter->resetXForm();
	painter->setPen( QColor( 200,200,200 ) );
	painter->drawText( rect(), Qt::AlignCenter, QString::number( d->value ) );
	painter->setPen( foregroundColor() );

	delete painter;
}

void QFloatPoti::wheelEvent( QWheelEvent* ev ) {
	float step = 0.05;
	if ( ev->delta() > 0 ) {
		setValue( d->value + step );
	} else {
		setValue( d->value - step );
	}
	repaint();
}

