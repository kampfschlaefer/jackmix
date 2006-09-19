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

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QMouseEvent>

using namespace JackMix;
using namespace JackMix::GUI;

Slider::Slider( float value, float min, float max, int precision, float pagestep, QWidget* p, QString valuestring, const char* n )
	: QWidget( p )
	, dB2VolCalc( min, max )
	, _value( value ), _pagestep( pagestep )
	, _value_inupdate( false )
	, _precision( precision )
	, _valuestring( valuestring )
{
	setMinimumSize( 20,20 );
	setFocusPolicy( Qt::TabFocus );
}
Slider::~Slider() {
}

void Slider::value( float n ) {
	if ( !_value_inupdate ) {
	//qDebug( "Slider::value( float %f )", n );
		if ( n>dbmax ) n=dbmax;
		if ( n<dbmin ) n=dbmin;
		_value = n;
		repaint();
		_value_inupdate=true;
		emit valueChanged( _value );
		_value_inupdate=false;
	}
}

void Slider::paintEvent( QPaintEvent* ) {
	bool rotated = false;
	QPainter p( this );

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) ) tmp = _valuestring.arg( tmp.left( tmp.indexOf( "." ) + _precision + 1 ) );
	else tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	int fontwidth = metrics.width( tmp );

	// Center the coordinates..
	p.translate( width()/2, height()/2 );

	int w = width();//-4;
	int h = height();//-4;
	if ( width() < height() ) {
		w = height();//-4;
		h = width();//-4;
		p.rotate( -90 );
		rotated = true;
	}

	if ( hasFocus() ) {
		//p.setPen( QPen( QPen::DotLine ) );
		//p.drawRect( -w/2-2, -h/2, w+4, h );
		style()->drawPrimitive( QStyle::PE_FrameFocusRect, 0, &p, this /*QRect( -w/2-2, -h/2, w+4, h )*//*, colorGroup()*/ );
	}

	float pos = dbtondb( _value )*w-w/2;
	// Rect for the bar
	p.fillRect( -w/2,-h/3, int( pos+w/2 ), h/3*2, QBrush( QColor( 0,150,0 ) ) /*, colorGroup().highlight()*/ );
	// Text showing the value
//	p.setPen( colorGroup().highlightedText() );
	p.drawText( -fontwidth/2, metrics.height()/2, tmp );
	// Top of the bar
//	p.setPen( colorGroup().highlightedText() );
	p.drawLine( int( pos ), -( h/3-1 ), int( pos ), h/3-2 );
	// Surrounding rect
//	p.setPen( colorGroup().foreground() );
	QRect tmp2 = QRect( -w/2, -h/3, w, h/3*2 );
	p.drawRect( tmp2 );
	_faderarea = p.matrix().mapRect( tmp2 );
}

void Slider::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == Qt::LeftButton && _faderarea.contains( ev->pos() ) )
		if ( width()>=height() )
			value( ndbtodb( ( ev->x() ) / float( _faderarea.width() ) ) );
		else
			value( ndbtodb( ( _faderarea.height() - ev->y() ) / float( _faderarea.height() ) ) );
}

void Slider::mouseMoveEvent( QMouseEvent* ev ) {
	if ( _faderarea.contains( ev->pos() ) )
		if ( width()>=height() )
			value( ndbtodb( ( ev->pos().x() ) / float( _faderarea.width() ) ) );
		else
			value( ndbtodb( ( _faderarea.height() - ev->pos().y() ) / float( _faderarea.height() ) ) );
}

void Slider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta()>0 )
		value( _value + _pagestep );
	else
		value( _value - _pagestep );
}

