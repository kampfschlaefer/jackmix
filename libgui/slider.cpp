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
#include <QtCore/QDebug>

using namespace JackMix;
using namespace JackMix::GUI;

Slider::Slider( float value, float min, float max, int precision, float pagestep, QWidget* p, QString valuestring )
	: QWidget( p )
	, dB2VolCalc( min, max )
	, _value( value ), _pagestep( pagestep )
	, _value_inupdate( false )
	, _precision( precision )
	, _valuestring( valuestring )
{
	setAutoFillBackground( false );
	int m = QFontMetrics( font() ).height();
	setMinimumSize( int( m*2.2 ), int( m*2.2 ) );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
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

#define SLIDER_BORDER 10

void Slider::paintEvent( QPaintEvent* ) {
	bool rotated = false;
	QPainter p( this );

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) ) tmp = _valuestring.arg( tmp.left( tmp.indexOf( "." ) + _precision + 1 ) );
	else tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	int fontwidth = metrics.width( tmp );

	// Center the coordinates
	p.translate( width()/2, height()/2 );

	int w = width()-SLIDER_BORDER;
	int h = height()-SLIDER_BORDER;
	if ( width() < height() ) {
		w = height()-SLIDER_BORDER;
		h = width()-SLIDER_BORDER;
		p.rotate( -90 );
		rotated = true;
	}

	if ( hasFocus() ) {
		style()->drawPrimitive( QStyle::PE_FrameFocusRect, 0, &p, this );
	}

	float pos = dbtondb( _value )*w-w/2;

	// Rect for the bar
	QRect bar( -w/2, -h/3, int( ceil( pos+w/2 ) ), h/3*2 );
	p.fillRect( bar, palette().color( QPalette::Highlight ) );

	p.setPen( palette().color( QPalette::WindowText ) );

	// Top of the bar
	QPen pen = p.pen();
	pen.setWidth( 2 );
	p.setPen( pen );
	p.drawLine( int( pos ), -( h/3 -1), int( pos ), h/3-1 );
	pen.setWidth( 1 );
	p.setPen( pen );

	// Surrounding rect
	QRect tmp2 = QRect( -w/2, -h/3, w, h/3*2 );
	p.drawRect( tmp2 );

	// Text showing the value
	p.drawText( -fontwidth/2, metrics.ascent()/2, tmp );
	p.setClipRect( bar );
	p.setPen( palette().color( QPalette::HighlightedText ) );
	p.drawText( -fontwidth/2, metrics.ascent()/2, tmp );

	// Set _faderarea correctly
	_faderarea = p.matrix().mapRect( tmp2 );
}

void Slider::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == Qt::LeftButton )
		mouseEvent( ev );
}

void Slider::mouseMoveEvent( QMouseEvent* ev ) {
	mouseEvent( ev );
}

void Slider::mouseEvent( QMouseEvent* ev ) {
	if ( width()>=height() )
		value( ndbtodb(
			( ev->pos().x() - _faderarea.x() ) / float( _faderarea.width()-1 )
			) );
	else
		value( ndbtodb(
			( _faderarea.height() - ev->pos().y() + _faderarea.y() ) / float( _faderarea.height()-1 )
			) );
}

void Slider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta()>0 )
		value( _value + _pagestep );
	else
		value( _value - _pagestep );
}

