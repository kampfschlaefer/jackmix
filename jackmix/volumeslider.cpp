
#include "volumeslider.h"
#include "volumeslider.moc"

#include <qlayout.h>
#include <qlabel.h>
#include "qfloatslider.h"

using namespace JackMix;

VolumeSlider::VolumeSlider( QString name, float value, QWidget* p, const char* n ) : QFrame( p,n ), _name( name ) {
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	setLineWidth( 2 );
	setMargin( 5 );
	QBoxLayout *_layout = new QBoxLayout( this, QBoxLayout::TopToBottom, 5, 5 );
	QFloatSlider *tmp = new QFloatSlider(  value,0,2,0.1,1000,Qt::Vertical,this );
	connect( tmp, SIGNAL( valueChanged( float ) ), this, SLOT( iValueChanged( float ) ) );
	_layout->addWidget( tmp,100 );
	_layout->addWidget( new QLabel( _name, this ),-10 );
}
VolumeSlider::~VolumeSlider() {
}

void VolumeSlider::iValueChanged( float n ) {
	emit valueChanged( _name, n );
}

