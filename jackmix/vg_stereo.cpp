
#include "vg_stereo.h"
#include "vg_stereo.moc"

#include "jack_backend.h"
#include "volumeslider.h"

#include <iostream>
#include <qlabel.h>
#include <qlayout.h>

using namespace JackMix;

VGStereo::VGStereo( QString _name, QObject* p, const char* n )
 : VolumeGroup( _name, 2, p,n )
{
std::cerr << "VGStereo::VGStereo( " << name() << ", " << p << ", n )" << std::endl;
	BACKEND->addOutput( name() + "_L" );
	BACKEND->addOutput( name() + "_R" );
}
VGStereo::~VGStereo() {
}

/*VolumeGroupMasterWidget* VGStereo::masterWidget( QWidget* parent ) {
	if ( !_masterwidget )
		_masterwidget = new VolumeGroupMasterWidget( this, parent );
	return _masterwidget;
}*/
VolumeGroupChannelWidget* VGStereo::channelWidget( QString name, QWidget* parent ) {
	return new VGStereoChannelWidget( name, this, parent );
}

//int VGStereo::channels() { return _channels; }

/*VolumeGroupMasterWidget::VolumeGroupMasterWidget( VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _group( group )
{
}
VolumeGroupMasterWidget::~VolumeGroupMasterWidget() {
}
*/

VGStereoChannelWidget::VGStereoChannelWidget( QString in, VolumeGroup* group, QWidget* p, const char* n )
 : VolumeGroupChannelWidget( in, group, p,n )
{
	setMargin( 2 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	QVBoxLayout* _layout = new QVBoxLayout( this );
	_layout->setMargin( 2 );
	_layout->addWidget( new QLabel( _in, this ) );
	QHBoxLayout* _layout2 = new QHBoxLayout( _layout );
	VolumeSlider* tmp;
	tmp = new VolumeSlider( _group->name() + "_L", 1, Qt::Vertical, this );
	connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( valueChanged( QString,float ) ) );
	_layout2->addWidget( tmp );
	tmp = new VolumeSlider( _group->name() + "_R", 1, Qt::Vertical, this );
	connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( valueChanged( QString,float ) ) );
	_layout2->addWidget( tmp );
}
VGStereoChannelWidget::~VGStereoChannelWidget() {
}
void VGStereoChannelWidget::valueChanged( QString channel, float value ) {
std::cerr << "VGStereoChannelWidget::valueChanged( " << channel << ", " << value << " )" << std::endl;
	BACKEND->setVolume( _in, channel, value );
}

