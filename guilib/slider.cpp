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

#include "slider.h"
#include "slider.moc"

#include <qpainter.h>

using namespace JackMix;
using namespace GUI;

Slider::Slider( float value, float min, float max, int precision, float pagestep, QWidget* p, QString valuestring, const char* n )
	: QWidget( p,n )
	, dB2VolCalc( min, max )
	, _value( value ), _pagestep( pagestep )
	, _precision( precision )
	, _valuestring( valuestring )
{
	setMinimumSize( 30,30 );
	setFocusPolicy( QWidget::TabFocus );
}
Slider::~Slider() {
}

void Slider::value( float n ) {
	static bool inupdate=false;
	if ( !inupdate ) {
		if ( n>dbmax ) n=dbmax;
		if ( n<dbmin ) n=dbmin;
		_value = n;
		repaint();
		inupdate=true;
		emit valueChanged( _value );
		inupdate=false;
	}
}

void Slider::paintEvent( QPaintEvent* ) {
	QPainter p( this );

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) ) tmp = _valuestring.arg( tmp.left( tmp.find( "." ) + _precision + 1 ) );
	else tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	int fontwidth = metrics.width( tmp );

	// Center the coordinates..
	p.translate( width()/2, height()/2 );

	int w = width()-4;
	int h = height()-4;
	if ( w < h ) {
		w = height()-4;
		h = width()-4;
		p.rotate( -90 );
	}

	if ( hasFocus() ) {
		p.setPen( QPen( QPen::DotLine ) );
		p.drawRect( -w/2-2, -h/2, w+4, h );
	}

	float pos = dbtondb( _value )*w-w/2;
	// Rect for the bar
	p.fillRect( -w/2,-h/3, int( pos+w/2 ), h/3*2, colorGroup().highlight() );
	// Text showing the value
	p.setPen( colorGroup().highlightedText() );
	p.drawText( -fontwidth/2, metrics.height()/2, tmp );
	// Top of the bar
	p.setPen( colorGroup().highlightedText() );
	p.drawLine( int( pos ), -( h/3-1 ), int( pos ), h/3-2 );
	// Surrounding rect
	p.setPen( colorGroup().foreground() );
	p.drawRect( -w/2,-h/3, w, h/3*2 );
}

void Slider::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == LeftButton )
		if ( width()>=height() )
			value( ndbtodb( ( ev->x() ) / float( width() ) ) );
		else
			value( ndbtodb( ( height() - ev->y() ) / float( height() ) ) );
	//qDebug( "_value=%f", _value );
}

void Slider::mouseMoveEvent( QMouseEvent* ev ) {
	//if ( ev->button() == LeftButton ) // Dont constrain to leftButton...
	if ( width()>=height() )
		value( ndbtodb( ( ev->pos().x() ) / float( width() ) ) );
	else
		value( ndbtodb( ( height() - ev->pos().y() ) / float( height() ) ) );
}

void Slider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta()>0 )
		value( _value + _pagestep );
	else
		value( _value - _pagestep );
}

