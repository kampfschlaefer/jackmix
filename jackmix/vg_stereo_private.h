
#ifndef JACKMIX_VOLUMEGROUP_STEREO_PRIVATE_H
#define JACKMIX_VOLUMEGROUP_STEREO_PRIVATE_H

#include "volumegroup.h"

namespace JackMix {

class VGStereoMasterWidget : public VolumeGroupMasterWidget {
Q_OBJECT
public:
	VGStereoMasterWidget( VGStereo*, QWidget* =0, const char* =0 );
	~VGStereoMasterWidget();
private slots:
	void valueChanged( QString, float );
};

class VGStereoChannelWidget : public VolumeGroupChannelWidget {
Q_OBJECT
public:
	VGStereoChannelWidget( QString, VolumeGroup*, QWidget*, const char* =0 );
	virtual ~VGStereoChannelWidget();
private slots:
	void valueChanged( QString, float );
};

};

#endif

