
#ifndef JACKMIX_VOLUMEGROUP_AUX_H
#define JACKMIX_VOLUMEGROUP_AUX_H

//#include <qframe.h>
//#include <qvaluevector.h>
#include "volumegroup.h"

namespace JackMix {

class VGAux : public VolumeGroup {
Q_OBJECT
public:
	/**
		\param QString name of the group
		\param int number of channels
	*/
	VGAux( QString, int, QObject* =0, const char* =0 );
	~VGAux();

	/// returns the masterwidget. The argument is used the first time for parenting the widget.
	//VolumeGroupMasterWidget* masterWidget( QWidget* );
	/**
		returns a new channelwidget.
		\param the name of the input channel
		\param The parent for the widget
	*/
	VolumeGroupChannelWidget* channelWidget( QString, QWidget* );

//	int channels();
//	QString name() { return _name; }
private:
/*	VolumeGroupMasterWidget* _masterwidget;
	QString _name;
	int _channels;*/
};

/*class VolumeGroupMasterWidget : public QFrame {
Q_OBJECT
public:
	VolumeGroupMasterWidget( VolumeGroup*, QWidget* =0, const char* =0 );
	~VolumeGroupMasterWidget();
private:
	VolumeGroup* _group;
};*/

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

