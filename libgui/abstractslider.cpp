
#include "abstractslider.h"
#include "abstractslider.moc"

#include <QtGui/QMouseEvent>
#include <QtGui/QDoubleSpinBox>

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

void AbstractSlider::hideInput() {
	if ( !_spinbox.isNull() ) {
		_spinbox->deleteLater();
	}
}

void AbstractSlider::mousePressEvent( QMouseEvent* ev ) {
	if ( ev->button() == Qt::LeftButton )
		mouseEvent( ev );
	else
		if ( ev->button() == Qt::RightButton ) {
			if ( _spinbox.isNull() ) {
				_spinbox = new QDoubleSpinBox( this );
				_spinbox->setMinimum( dbmin );
				_spinbox->setMaximum( dbmax );
				_spinbox->setSingleStep( _pagestep );
				_spinbox->setValue( _value );
				connect( _spinbox, SIGNAL( editingFinished() ), this, SLOT( hideInput() ) );
				connect( _spinbox, SIGNAL( valueChanged( double ) ), this, SLOT( value( double ) ) );
				connect( this, SIGNAL( valueChanged( double ) ), _spinbox, SLOT( setValue( double ) ) );
				_spinbox->show();
				_spinbox->move( ( width()-_spinbox->width() )/2, ( height()-_spinbox->height() )/2 );
			}
			ev->accept();
		}
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
