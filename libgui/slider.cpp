/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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
#include <QtCore/QTimer>

using namespace JackMix;
using namespace JackMix::GUI;

Slider::Slider( double value, double min, double max, int precision, double pagestep, QWidget* p, QString valuestring )
	: AbstractSlider( value, min, max, precision, pagestep, p, valuestring )
	, _timer( new QTimer( this ) )
	, _show_value( false )
{
	setAutoFillBackground( false );
	int m = QFontMetrics( font() ).height();
	int w = QFontMetrics( font() ).width( valuestring );
	setMinimumSize( int( w*1.2 ), int( m*2.2 ) );
	setFocusPolicy( Qt::TabFocus );

	_timer->setInterval( 2000 );
	_timer->setSingleShot( true );
	connect( _timer, SIGNAL( timeout() ), this, SLOT( timeout() ) );
}
Slider::~Slider() {
}

void Slider::value( double n ) {
	if ( !_value_inupdate ) {
		AbstractSlider::value( n );
		_show_value = true;
		_timer->start();
	}
}
void Slider::timeout() {
	_show_value = false;
	update();
}

#define SLIDER_BORDER 5

void Slider::paintEvent( QPaintEvent* ) {
	bool rotated = false;
	QPainter p( this );
	p.setRenderHints( QPainter::Antialiasing );

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) ) tmp = _valuestring.arg( tmp.left( tmp.indexOf( "." ) + _precision + 1 ) );
	else tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	int fontwidth = metrics.width( tmp );

	// Center the coordinates
	p.translate( width()/2, height()/2 );

	double w = width()-SLIDER_BORDER;
	double h = height()-SLIDER_BORDER;
	if ( width() < height() ) {
		w = height()-SLIDER_BORDER;
		h = width()-SLIDER_BORDER;
		p.rotate( -90 );
		rotated = true;
	}

	if ( hasFocus() ) {
		style()->drawPrimitive( QStyle::PE_FrameFocusRect, 0, &p, this );
	}

	//double pos = dbtondb( _value )*w-w/2;
	QRectF bar( -w/2, -h/4, w, h/2 );

	// Tickmarks
	p.save();
	p.setPen( palette().color( QPalette::ButtonText ) );
	QFont small = font();
	small.setPointSizeF( qMax( 5.0, font().pointSizeF()/2 ) );
	p.setFont( small );
	if ( _show_value ) {
		for ( double a=_pagestep; a<dbmax; a+=_pagestep )
			p.drawLine( QPointF( -w/2+w*dbtondb( a ), h/3.5 ), QPointF( -w/2+w*dbtondb( a ), -h/3.5 ) );
		for ( double a=-_pagestep; a>dbmin; a-=_pagestep )
			p.drawLine( QPointF( -w/2+w*dbtondb( a ), h/3.5 ), QPointF( -w/2+w*dbtondb( a ), -h/3.5 ) );
	}
	QList<double> _texts;
	_texts << dbmin << dbmax;
	if ( 0>dbmin && 0<dbmax )
		_texts << 0.0;
	foreach( double a, _texts ) {
		p.save();
		p.translate( -w/2+w*dbtondb( a ), 0 );
		p.drawLine( QPointF( 0, h/3 ), QPointF( 0, -h/3 ) );
		QRectF rect(
			0,0,
			QFontMetrics( small ).width( _valuestring ),
			QFontMetrics( small ).height() );
		if ( !rotated ) {
			if ( dbtondb( a ) > 0.5 )
				rect.translate( -QFontMetrics( small ).width( _valuestring ), 0 );
			p.drawText( rect.translated( 0, h/3 ), Qt::AlignCenter, QString( _valuestring ).arg( a ) );
			//p.drawText( rect.translated( 0, -h/3 -rect.height() ), Qt::AlignCenter, QString( _valuestring ).arg( a ) );
			if ( a == 0.0 ) {
				_nullclick = p.matrix().mapRect( rect.translated( 0, h/3 ) ).toRect();
				//_nullclick = _nullclick.united( p.matrix().mapRect( rect.translated( 0, -h/3 -rect.height() ) ).toRect() );
			}
		} else {
			p.rotate( 90 );
			if ( dbtondb( a ) < 0.5 )
				rect.translate( 0, -QFontMetrics( small ).height() );
			p.drawText( rect.translated( h/3, 0 ), Qt::AlignCenter, QString( _valuestring ).arg( a ) );
			//p.drawText( rect.translated( -h/3 -rect.width(), 0 ), Qt::AlignCenter, QString( _valuestring ).arg( a ) );
			if ( a == 0.0 ) {
				_nullclick = p.matrix().mapRect( rect.translated( h/3,0 ) ).toRect();
				//_nullclick = _nullclick.united( p.matrix().mapRect( rect.translated( -h/3-rect.width(),0 ) ).toRect() );
			}
		}
		p.restore();
	}
	p.restore();

	// Surrounding rect
	p.drawRect( bar );

	// Rect for the whole bar
	{
		p.save();
		QLinearGradient grad( QPointF( -w/2, -h/4 ), QPointF( w/2, -h/4 ) );
		// Global ends first
		grad.setColorAt( 0.0, palette().color( QPalette::Highlight ) );
		if ( dbtondb( _value ) < 1.0 )
			grad.setColorAt( 1.0, palette().color( QPalette::Highlight ).dark() );
		// Next soft-fades
		grad.setColorAt( qMax( 0.0, dbtondb( _value )-0.01 ), palette().color( QPalette::Highlight ).light() );
		if ( dbtondb( _value )+0.01 < 1.0 )
		grad.setColorAt( qMin( 1.0, dbtondb( _value )+0.01 ), palette().color( QPalette::Highlight ) );
		// Last the value itself
		grad.setColorAt( dbtondb( _value ), palette().color( QPalette::HighlightedText ) );
		// That way minimum and maximum get the right color
		p.fillRect( bar, grad );
		p.restore();
	}

	// Set _faderarea correctly
	_faderarea = p.matrix().mapRect( bar ).toRect();

	// de-rotate
	if ( rotated )
		p.rotate( 90 );

	// Draw the value as text in the lower third
	if ( _show_value ) {
		p.save();
		p.setPen( Qt::NoPen );
		p.setBrush( palette().color( QPalette::Base ) );
		p.setOpacity( 0.75 );
		p.drawRoundRect( -fontwidth/2 -2, height()/3 -metrics.ascent() -1, fontwidth +4, metrics.ascent() +4 );
		p.restore();
		// Text showing the value
		p.drawText( -fontwidth/2, height()/3, tmp );
	}
}


void Slider::mouseEvent( QMouseEvent* ev ) {
	if ( width()>=height() )
		value( ndbtodb(
			( ev->pos().x() - _faderarea.x() ) / double( _faderarea.width()-1 )
			) );
	else
		value( ndbtodb(
			( _faderarea.height() - ev->pos().y() + _faderarea.y() ) / double( _faderarea.height()-1 )
			) );
}

