
#include "volumegroup.h"
#include "volumegroup.moc"

#include "jack_backend.h"
#include "volumeslider.h"

#include <iostream>
#include <qlabel.h>
#include <qlayout.h>

using namespace JackMix;

VolumeGroupFactory::VolumeGroupFactory() {
}
VolumeGroupFactory::~VolumeGroupFactory() {
}
VolumeGroupFactory* VolumeGroupFactory::the() {
	static VolumeGroupFactory* factory = new VolumeGroupFactory();
	return factory;
}
void VolumeGroupFactory::registerGroup( VolumeGroup* n ) {
	_groups.push_back( n );
}

VolumeGroup::VolumeGroup( QString name, int channels, QObject* p, const char* n )
 : QObject( p,n )
 , _masterwidget( 0 )
 , _name( name )
 , _channels( channels )
{
	VolumeGroupFactory::the()->registerGroup( this );
	for ( int i=0; i<_channels; i++ ) {
		BACKEND->addOutput( _name + QString::number( i ) );
	}
}
VolumeGroup::~VolumeGroup() {
}

VolumeGroupMasterWidget* VolumeGroup::masterWidget( QWidget* parent ) {
	if ( !_masterwidget )
		_masterwidget = new VolumeGroupMasterWidget( this, parent );
	return _masterwidget;
}
VolumeGroupChannelWidget* VolumeGroup::channelWidget( QString name, QWidget* parent ) {
	return new VolumeGroupChannelWidget( name, this, parent );
}

int VolumeGroup::channels() { return _channels; }

VolumeGroupMasterWidget::VolumeGroupMasterWidget( VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _group( group )
{
}
VolumeGroupMasterWidget::~VolumeGroupMasterWidget() {
}


VolumeGroupChannelWidget::VolumeGroupChannelWidget( QString in, VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _in( in )
 , _group( group )
{
	setMargin( 2 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	QVBoxLayout* _layout = new QVBoxLayout( this );
	_layout->setMargin( 2 );
	_layout->addWidget( new QLabel( _in, this ) );
	QHBoxLayout* _layout2 = new QHBoxLayout( _layout );
	VolumeSlider* tmp;
	for ( int i=0; i<_group->channels(); i++ ) {
		tmp = new VolumeSlider( _group->name() + QString::number( i ), 1, this );
		connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( valueChanged( QString,float ) ) );
		_layout2->addWidget( tmp );
	}
}
VolumeGroupChannelWidget::~VolumeGroupChannelWidget() {
}
void VolumeGroupChannelWidget::valueChanged( QString channel, float value ) {
	//std::cerr << "VolumeGroupChannelWidget::valueChanged( " << channel << ", " << value << " )" << std::endl;
	BACKEND->setVolume( _in, channel, value );
}

