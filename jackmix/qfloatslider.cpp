
#include "qfloatslider.h"
#include "qfloatslider.moc"

#include <qslider.h>
#include <qlayout.h>

QFloatSlider::QFloatSlider( QWidget* p, const char* n ) : QFrame( p,n ) {
	init( 0,0,0,0,1,JackMix::LeftToRight );
}
QFloatSlider::QFloatSlider( JackMix::Direction o, QWidget* p, const char* n ) : QFrame( p,n ) {
	init( 0,0,0,0,1,o );
}
QFloatSlider::QFloatSlider( float value, float min, float max, float pagestep, int precision, JackMix::Direction o, QWidget* p, const char* n ) : QFrame( p,n ) {
	init( value, min, max, pagestep, precision, o );
}

QFloatSlider::~QFloatSlider() {
}

void QFloatSlider::init( float value, float min, float max, float pagestep, int precision, JackMix::Direction dir ) {
	_dir = dir;
	//setFrameStyle( QFrame::Panel|QFrame::Sunken );
	//setMargin( 1 );
	//setLineWidth( 1 );
	QBoxLayout* _layout = new QBoxLayout( this, QBoxLayout::LeftToRight, 2 );
	if ( _dir == JackMix::LeftToRight || _dir == JackMix::RightToLeft )
		_slider = new QSlider( Qt::Horizontal, this );
	else
		_slider = new QSlider( Qt::Vertical, this );
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
	setMinMax();
//	_slider->setMinValue( int( _min*_precision ) );
}
void QFloatSlider::setMaximum( float n ) {
	_max = n;
	setMinMax();
//	_slider->setMaxValue( int( _max*_precision ) );
}
void QFloatSlider::setValue( float n ) {
	_value = n;
	_slider->setValue( int( _value*_precision ) );
}
void QFloatSlider::setValue( int n ) {
	_value = n / float( _precision );
	if ( ( _dir == JackMix::RightToLeft ) || ( _dir == JackMix::BottomToTop ) )
		_value *= -1;
	emit valueChanged( _value );
}
void QFloatSlider::setMinMax() {
	if ( ( _dir == JackMix::LeftToRight ) || ( _dir == JackMix::TopToBottom ) ) {
		_slider->setRange( int( _min*_precision ), int( _max*_precision ) );
	} else {
		_slider->setRange( -int( _max*_precision ), -int( _min*_precision ) );
	}
}

