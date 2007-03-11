/*
    Copyright ( C ) 2006 Arnold Krille <arnold@arnoldarts.de>

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

Knob::Knob( float v, float min, float max, int precision, float pagestep, QWidget*p, QString valuestring )
	: QWidget( p )
	, dB2VolCalc( min, max )
	, _value( v )
	, _pagestep( pagestep )
	, _value_inupdate( false )
	, _precision( precision )
	, _valuestring( valuestring )
{
	//qDebug() << "Knob::Knob(" << v << "," << min << "," << max << "," << precision << "," << pagestep << "," << p << "," << valuestring << ")";
	int m = QFontMetrics( font() ).width( _valuestring ) + ( _precision+1 )* QFontMetrics( font() ).width( " " );
	setMinimumSize( int( m*1.1 ), int( m*1.1 ) );
	setFocusPolicy( Qt::TabFocus );
}

Knob::~Knob() {
}

void Knob::value( float n ) {
	if ( !_value_inupdate ) {
		n = qMin( n, dbmax );
		n = qMax( n, dbmin );
		_value = n;
		update();
		_value_inupdate = true;
		emit valueChanged( n );
		_value_inupdate = false;
	}
}

void Knob::paintEvent( QPaintEvent* ) {
	QPainter p( this );
	p.setRenderHints( QPainter::Antialiasing );

	p.translate( width()/2.0, height()/2.0 );

	p.save();

	p.rotate( -240 );

	float radius = qMin( width(), height() ) /2 -10; // "Border" of 10 pixels

	// Draw Arc around whole area
	//p.drawArc( QRectF( -radius, -radius, 2*radius, 2*radius ), 16* 0, 16* -300 );

	// Draw Knob
	p.save();
	p.setPen( Qt::NoPen );
	{
		QRadialGradient grad( QPointF( 0,0 ), 2*radius, QPointF( 0, radius*0.8 ) );
		grad.setColorAt( 0, palette().color( QPalette::Highlight ) );
		grad.setColorAt( 1, palette().color( QPalette::Highlight ).dark() );
		grad.setSpread( QGradient::PadSpread );
		p.setBrush( grad );
		p.drawEllipse( QRectF( -radius*0.8, -radius*0.8, radius*1.6, radius*1.6 ) );
	}
	{
		QRadialGradient grad( QPointF( 0,0 ), radius*0.65, QPointF( 0, radius*0.45 ) );
		grad.setColorAt( 1, palette().color( QPalette::Highlight ) );
		grad.setColorAt( 0, palette().color( QPalette::Highlight ).light() );
		grad.setSpread( QGradient::PadSpread );
		p.setBrush( grad );
		p.drawEllipse( QRectF( -radius*0.7, -radius*0.7, radius*1.4, radius*1.4 ) );
	}
	p.restore();

	// Draw ticks
	p.save();
	p.setPen( palette().color( QPalette::ButtonText ) );
	for ( int i=0; i<=300; i+=10 ) {
		p.drawLine( QPointF( radius*0.80, 0 ), QPointF( radius*0.85, 0 ) );
		p.rotate( 10 );
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

	QString tmp = QString::number( _value );
	if ( tmp.contains( "." ) ) tmp = _valuestring.arg( tmp.left( tmp.indexOf( "." ) + _precision + 1 ) );
	else tmp = _valuestring.arg( tmp );

	QFontMetrics metrics( font() );
	QRectF rect = metrics.boundingRect( tmp );
	double x = rect.width() /2;
	{
		// Draw a 50% transparent rect behind the text...
		p.save();
		p.setPen( Qt::NoPen );
		p.setBrush( palette().color( QPalette::Window ) );
		p.setOpacity( 0.5 );
		p.drawRect( rect.translated( -x, radius*0.9 ).adjusted( -2,-2, 2,0 ) );
		p.restore();
	}
	p.drawText( QPointF( -x, radius*0.9 ), tmp );
}

void Knob::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == Qt::LeftButton )
		mouseEvent( ev );
}
void Knob::mouseMoveEvent( QMouseEvent* ev ) {
	mouseEvent( ev );
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

void Knob::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta() > 0 )
		value( _value + _pagestep );
	else
		value( _value - _pagestep );
}

}; // GUI
}; // JackMix
