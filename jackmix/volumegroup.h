
#ifndef JACKMIX_VOLUMEGROUP_H
#define JACKMIX_VOLUMEGROUP_H

#include <qframe.h>
#include <qvaluevector.h>

namespace JackMix {

class VolumeGroup;
class VolumeGroupMasterWidget;
class VolumeGroupChannelWidget;

class VolumeGroupFactory {
private:
	VolumeGroupFactory();
	~VolumeGroupFactory();
	QValueVector <VolumeGroup*> _groups;
public:
	static VolumeGroupFactory* the();
	void registerGroup( VolumeGroup* );
	int groups() { return _groups.size(); }
	VolumeGroup* group( int n ) { return _groups[ n ]; }
};

class VolumeGroup : public QObject {
Q_OBJECT
public:
	/**
		\param QString name of the group
		\param int number of channels
	*/
	VolumeGroup( QString, int, QObject* =0, const char* =0 );
	virtual ~VolumeGroup();

	/// returns the masterwidget. The argument is used the first time for parenting the widget.
	virtual VolumeGroupMasterWidget* masterWidget( QWidget* );
	/**
		returns a new channelwidget.
		\param the name of the input channel
		\param The parent for the widget
	*/
	virtual VolumeGroupChannelWidget* channelWidget( QString, QWidget* );

	virtual int channels();
	QString name() { return _name; }
private:
	QString _name;
	int _channels;
private:
	VolumeGroupMasterWidget* _masterwidget;
};

class VolumeGroupMasterWidget : public QFrame {
Q_OBJECT
public:
	VolumeGroupMasterWidget( VolumeGroup*, QWidget* =0, const char* =0 );
	~VolumeGroupMasterWidget();
protected:
	VolumeGroup* _group;
};

class VolumeGroupChannelWidget : public QFrame {
Q_OBJECT
public:
	VolumeGroupChannelWidget( QString, VolumeGroup*, QWidget*, const char* =0 );
	virtual ~VolumeGroupChannelWidget();
	QString groupname() { return _group->name(); }
protected:
	QString _in;
	VolumeGroup* _group;
};

};

#endif

