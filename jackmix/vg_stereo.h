
#ifndef JACKMIX_VOLUMEGROUP_STEREO_H
#define JACKMIX_VOLUMEGROUP_STEREO_H

#include "volumegroup.h"

namespace JackMix {

class VGStereo : public VolumeGroup {
Q_OBJECT
public:
	/**
		\param QString name of the group
		\param int number of channels
	*/
	VGStereo( QString, QObject* =0, const char* =0 );
	virtual ~VGStereo();

	/// returns the masterwidget. The argument is used the first time for parenting the widget.
	VolumeGroupMasterWidget* masterWidget( QWidget* );
	/**
		returns a new channelwidget.
		\param the name of the input channel
		\param The parent for the widget
	*/
	VolumeGroupChannelWidget* channelWidget( QString, QWidget* );

};

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

