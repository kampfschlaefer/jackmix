
#include "qfloatslider.h"
#include "qfloatslider.moc"

#include <qslider.h>
#include <qlayout.h>

QFloatSlider::QFloatSlider( QWidget* p, const char* n ) : QFrame( p,n ) {
	init( 0,0,0,0,1,Qt::Horizontal );
}
QFloatSlider::QFloatSlider( Orientation o, QWidget* p, const char* n ) : QFrame( p,n ) {
	init( 0,0,0,0,1,o );
}
QFloatSlider::QFloatSlider( float value, float min, float max, float pagestep, int precision, Orientation o, QWidget* p, const char* n ) : QFrame( p,n ) {
	init( value, min, max, pagestep, precision, o );
}

QFloatSlider::~QFloatSlider() {
}

void QFloatSlider::init( float value, float min, float max, float pagestep, int precision, Orientation o ) {
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	setMargin( 1 );
	setLineWidth( 2 );
	QBoxLayout* _layout = new QBoxLayout( this, QBoxLayout::LeftToRight, 2 );
	_slider = new QSlider( o, this );
	_layout->addWidget( _slider );
	setPrecision( precision );
	setValue( value );
	setMinimum( min );
	setMaximum( max );
	setPageStep( pagestep );
	connect( _slider, SIGNAL( valueChanged( int ) ), this, SLOT( setValue( int ) ) );
}

void QFloatSlider::setPrecision( int n ) {
	_precision = n;
}
void QFloatSlider::setPageStep( float n ) {
	_pagestep = n;
	_slider->setPageStep( int( _pagestep*_precision ) );
}
void QFloatSlider::setMinimum( float n ) {
	_min = n;
	_slider->setMinValue( int( _min*_precision ) );
}
void QFloatSlider::setMaximum( float n ) {
	_max = n;
	_slider->setMaxValue( int( _max*_precision ) );
}
void QFloatSlider::setValue( float n ) {
	_value = n;
	_slider->setValue( int( _value*_precision ) );
}
void QFloatSlider::setValue( int n ) {
	_value = n / float( _precision );
	emit valueChanged( _value );
}
