
#include "vg_aux.h"
#include "vg_aux.moc"

#include "jack_backend.h"
#include "volumeslider.h"

#include <iostream>
#include <qlabel.h>
#include <qlayout.h>

using namespace JackMix;

VGAux::VGAux( QString _name, int ch, QObject* p, const char* n )
 : VolumeGroup( _name, ch, p,n )
{
std::cerr << "VGAux::VGAux( " << name() << ", " << channels() << ", " << p << ", n )" << std::endl;
	for ( int i=0; i<channels(); i++ ) {
		BACKEND->addOutput( name() + "_" + QString::number( i ) );
	}
}
VGAux::~VGAux() {
}

/*VolumeGroupMasterWidget* VolumeGroup::masterWidget( QWidget* parent ) {
	if ( !_masterwidget )
		_masterwidget = new VGAuxMasterWidget( this, parent );
	return _masterwidget;
}*/
VolumeGroupChannelWidget* VGAux::channelWidget( QString name, QWidget* parent ) {
	return new VGAuxChannelWidget( name, this, parent );
}

//int VolumeGroup::channels() { return _channels; }

/*VolumeGroupMasterWidget::VolumeGroupMasterWidget( VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _group( group )
{
}
VolumeGroupMasterWidget::~VolumeGroupMasterWidget() {
}*/


VGAuxChannelWidget::VGAuxChannelWidget( QString in, VGAux* group, QWidget* p, const char* n )
 : VolumeGroupChannelWidget( in, group, p,n )
 , _in( in )
{
	setMargin( 1 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	QVBoxLayout* _layout = new QVBoxLayout( this );
	VolumeSlider* tmp;
	for ( int i=0; i<_group->channels(); i++ ) {
		tmp = new VolumeSlider( _group->name() + "_" + QString::number( i ), 1, Qt::Horizontal, this );
		connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( valueChanged( QString,float ) ) );
		_layout->addWidget( tmp );
	}
}
VGAuxChannelWidget::~VGAuxChannelWidget() {
}
void VGAuxChannelWidget::valueChanged( QString channel, float value ) {
	//std::cerr << "VolumeGroupChannelWidget::valueChanged( " << channel << ", " << value << " )" << std::endl;
	BACKEND->setVolume( _in, channel, value );
}

