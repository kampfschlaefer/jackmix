
#include "volumegroup.h"
#include "volumegroup.moc"

using namespace JackMix;

VolumeGroup::VolumeGroup( QObject* p, const char* n ) : QObject( p,n ), _masterwidget( 0 ) {
}
VolumeGroup::~VolumeGroup() {
}

VolumeGroupMasterWidget* VolumeGroup::masterWidget( QWidget* parent ) {
	if ( !_masterwidget )
		_masterwidget = new VolumeGroupMasterWidget( this, parent );
	return _masterwidget;
}

VolumeGroupMasterWidget::VolumeGroupMasterWidget( VolumeGroup* group, QWidget* p, const char* n )
 : QFrame( p,n )
 , _group( group )
{
}
VolumeGroupMasterWidget::~VolumeGroupMasterWidget() {
}

