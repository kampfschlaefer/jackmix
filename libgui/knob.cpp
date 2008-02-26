/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library. If not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#include "knob.h"
#include "knob.moc"

#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtCore/QDebug>
#include <QtCore/QTimer>

namespace JackMix {
namespace GUI {

qreal abs( const QPointF p ) {
	return sqrt( p.x()*p.x() + p.y()*p.y() );
}
qreal sgn( const qreal n ) {
	if ( n<0 )
		return -1;
	return 1;
}

double degree( const double n ) {
	if ( n > 360 )
		return n - 360;
	return n;
}

Knob::Knob( double v, double min, double max, int precision, double pagestep, QWidget*p, QString valuestring )
	: AbstractSlider( v, min, max, precision, pagestep, p, valuestring )
	, _timer( new QTimer( this ) )
	, _show_value( false )
{
	int m = fontMetrics().width( _valuestring ) + ( _precision+2 )* QFontMetrics( font() ).width( " " );
	int h = fontMetrics().height();
	setMinimumSize( int( m*1.1 ), int( h*2.2 ) );
	setFocusPolicy( Qt::TabFocus );

	_timer->setInterval( 2000 ); // 2 seconds timeout for showing the value
	_timer->setSingleShot( true );
	connect( _timer, SIGNAL( timeout() ), this, SLOT( timeOut() ) );
}

Knob::~Knob() {
}

void Knob::value( double n ) {
	if ( !_value_inupdate ) {
		AbstractSlider::value( n );
		_show_value = true;
		_timer->start();
	}
}

void Knob::timeOut() {
	_show_value = false;
	update();
}

void Knob::paintEvent( QPaintEvent* ) {
	QPainter p( this );
	p.setRenderHints( QPainter::Antialiasing );

	p.translate( width()/2.0, height()/2.0 );

	p.save();

	p.rotate( -240 );

	double radius = qMin( width(), height() ) /2 -4; // "Border" of 10 pixels

	// Draw Arc around whole area
	//p.drawArc( QRectF( -radius, -radius, 2*radius, 2*radius ), 16* 0, 16* -300 );

	// Draw Knob
	p.save();
	p.setPen( Qt::NoPen );
	{
		QRadialGradient grad( QPointF( 0,0 ), radius, QPointF( 0, radius*0.7 ) );
		grad.setColorAt( 0, palette().color( QPalette::Highlight ) );
		grad.setColorAt( 1, palette().color( QPalette::Highlight ).dark() );
		grad.setSpread( QGradient::PadSpread );
		p.setBrush( grad );
		p.drawEllipse( QRectF( -radius*0.8, -radius*0.8, radius*1.6, radius*1.6 ) );
	}
	{
		QRadialGradient grad( QPointF( 0,0 ), radius*0.60, QPointF( 0, radius*0.20 ) );
		grad.setColorAt( 1, palette().color( QPalette::Highlight ) );
		grad.setColorAt( 0, palette().color( QPalette::Highlight ).light() );
		grad.setSpread( QGradient::PadSpread );
		p.setBrush( grad );
		p.drawEllipse( QRectF( -radius*0.65, -radius*0.65, radius*1.3, radius*1.3 ) );
	}
	p.restore();

	// Draw ticks
	p.save();
	p.setPen( palette().color( QPalette::ButtonText ) );
	if ( _show_value ) {
		for ( double a=_pagestep; a<dbmax; a+= _pagestep ) {
			p.save();
			p.rotate( dbtondb( a )*300 );
			p.drawLine( QPointF( radius*0.80,0 ), QPointF( radius*0.85,0 ) );
			p.restore();
		}
		for ( double a=-_pagestep; a>dbmin; a-= _pagestep ) {
			p.save();
			p.rotate( dbtondb( a )*300 );
			p.drawLine( QPointF( radius*0.80,0 ), QPointF( radius*0.85,0 ) );
			p.restore();
		}
	}
	QList<double> _texts;
	_texts << dbmin << dbmax;
	if ( 0<dbmax && 0>dbmin )
		_texts << 0.0;
	QFont small = font();
	small.setPointSizeF( qMax( 5.0, font().pointSizeF()/2 ) );
	p.setFont( small );
	foreach( double a, _texts ) {
		p.save();
		p.rotate( dbtondb( a )*300 );
		p.drawLine( QPointF( radius*0.80,0 ), QPointF( radius*0.92,0 ) );
		p.translate( QPointF( radius*0.91,0 ) );
		p.rotate( -dbtondb( a )*300 +240 );
		QRectF rect(
			0,0,
			QFontMetrics( small ).width( _valuestring ),
			QFontMetrics( small ).height() );
		if ( dbtondb( a ) < 0.5 )
			rect.translate( -QFontMetrics( small ).width( _valuestring ), 0 );
		p.drawText( rect, Qt::AlignCenter, QString( _valuestring ).arg( a ) );
		if ( a == 0.0 )
			_nullclick = p.matrix().mapRect( rect ).toRect();
		p.restore();
	}
	p.restore();

	// Draw highlight-line for the value
	p.save();
	p.rotate( 300 * dbtondb( _value ) );
	QPen linepen( palette().color( QPalette::HighlightedText ) );
	linepen.setWidthF( 3 );
	linepen.setCapStyle( Qt::RoundCap );
	p.setPen( linepen );
	p.drawLine( QPointF( 0,0 ), QPointF( radius*0.71,0 ) );
	p.restore();

	p.restore();

	if ( _show_value ) {

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) )
		tmp = _valuestring.arg( tmp.left( tmp.indexOf( "." ) + _precision + 1 ) );
	else
		tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	QRectF rect = metrics.boundingRect( tmp );
	double x = rect.width() /2;
	double y = qMax( int( radius ), height()/2 );
	{
		// Draw a 50% transparent rect behind the text...
		p.save();
		p.setPen( Qt::NoPen );
		//p.setBrush( palette().color( QPalette::Window ) );
		p.setOpacity( 0.75 );
		p.setBrush( palette().color( QPalette::Base ) );
		p.drawRoundRect( rect.translated( -x, y*0.9 ).adjusted( -2, 1, 2,-2 ), 20, 50 );
		p.restore();
	}
	p.drawText( QPointF( -x, y*0.9 ), tmp );

	}

}

void Knob::mouseEvent( QMouseEvent* ev ) {
	//qDebug() << "Knob::mouseEvent(" << ev << ")";
	//qDebug() << " ev->x" << ev->x() << " ev->y" << ev->y();
	QPointF pos( ev->pos() );
	QPointF middle( width()/2.0, height()/2.0 );
	QPointF rpos( pos-middle );
	if ( abs( rpos ) > 10 ) {
		//qDebug() << " " << pos << " - " << middle << " = " << rpos;
		//qDebug() << " length is" << abs( rpos );
		double alpha_sin = asin( rpos.y()/ abs( rpos ) )/M_PI*180;
		//qDebug() << " asin(rpos)" << alpha_sin;
		double alpha_cos = acos( rpos.x()/ abs( rpos ) )/M_PI*180;
		//qDebug() << " acos(rpos)" << alpha_cos;
		double alpha = alpha_cos * sgn( alpha_sin );
		//qDebug() << " alpha" << alpha;
		alpha = degree( alpha + 240 );
		if ( alpha > 330 )
			alpha = 0;
		alpha = qMin( alpha, 300.0 );
		//qDebug() << " alpha" << alpha;

		//qDebug() << " value =" << alpha/300 << " ndbtodb =" << ndbtodb( alpha/300 );
		value( ndbtodb( alpha/300 ) );
	}
}


}; // GUI
}; // JackMix
