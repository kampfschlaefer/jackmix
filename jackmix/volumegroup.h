
#ifndef JACKMIX_VOLUMEGROUP_H
#define JACKMIX_VOLUMEGROUP_H

#include <qframe.h>

namespace JackMix {

class VolumeGroupMasterWidget;

class VolumeGroup : public QObject {
Q_OBJECT
public:
	VolumeGroup( QObject* =0, const char* =0 );
	~VolumeGroup();

	VolumeGroupMasterWidget* masterWidget( QWidget* );
private:
	VolumeGroupMasterWidget* _masterwidget;
};

class VolumeGroupMasterWidget : public QFrame {
Q_OBJECT
public:
	VolumeGroupMasterWidget( VolumeGroup*, QWidget* =0, const char* =0 );
	~VolumeGroupMasterWidget();
private:
	VolumeGroup* _group;
};

class VolumeGroupChannelWidget : public QFrame {
Q_OBJECT
};

};

#endif

