
#ifndef JACKMIX_VOLUMEGROUP_AUX_PRIVATE_H
#define JACKMIX_VOLUMEGROUP_AUX_PRIVATE_H

#include "volumegroup.h"

namespace JackMix {

class VGAuxMasterWidget : public VolumeGroupMasterWidget {
Q_OBJECT
public:
	VGAuxMasterWidget( VGAux*, QWidget* =0, const char* =0 );
	~VGAuxMasterWidget();
private slots:
	void newValue( QString, float );
};

class VGAuxChannelWidget : public VolumeGroupChannelWidget {
Q_OBJECT
public:
	VGAuxChannelWidget( QString, VGAux*, QWidget*, const char* =0 );
	~VGAuxChannelWidget();
private slots:
	void valueChanged( QString, float );
private:
	QString _in;
};

};

#endif

