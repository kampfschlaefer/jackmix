
#include "abstractslider.h"
#include "abstractslider.moc"

#include <QtGui/QMouseEvent>

namespace JackMix {
namespace GUI {

AbstractSlider::AbstractSlider( double value, double min, double max, int precision, double pagestep, QWidget* p, QString valuestring )
	: QWidget( p )
	, dB2VolCalc( min, max )
	, _value( value ), _value_inupdate( false ), _pagestep( pagestep )
	, _precision( precision )
	, _valuestring( valuestring )
{
}
AbstractSlider::~AbstractSlider() {
}

void AbstractSlider::value( double v ) {
	if ( !_value_inupdate ) {
		v = qMin( v, dbmax );
		v = qMax( v, dbmin );
		_value = v;
		update();
		_value_inupdate = true;
		emit valueChanged( _value );
		_value_inupdate = false;
	}
}
void AbstractSlider::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == Qt::LeftButton )
		mouseEvent( ev );
}
void AbstractSlider::mouseMoveEvent( QMouseEvent* ev ) {
	mouseEvent( ev );
}
void AbstractSlider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta() > 0 )
		value( value() + _pagestep );
	else
		value( value() - _pagestep );
}

};
};
