/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#ifndef JACKMIX_VOLUMEGROUP_H
#define JACKMIX_VOLUMEGROUP_H

#include <qframe.h>
#include <qvaluevector.h>
#include <qptrlist.h>

namespace JackMix {

class VolumeGroup;
class VolumeGroupMasterWidget;
class VolumeGroupChannelWidget;

class VolumeGroupFactory : public QObject {
Q_OBJECT
private:
	VolumeGroupFactory();
	~VolumeGroupFactory();
	QPtrList <VolumeGroup> _groups;
signals:
	/// Is emitted
	void sNewVG( VolumeGroup* );
	void sRemoveVG( VolumeGroup* );
public:
	static VolumeGroupFactory* the();
	void registerGroup( VolumeGroup* );
	void unregisterGroup( VolumeGroup* );
	int groups() { return _groups.count(); }
	VolumeGroup* group( int n ) { return _groups.at( n ); }
};

class VolumeGroup : public QObject {
Q_OBJECT
public:
	/**
	 * What implementers have to do:
	 *   - Create the needed Channels
	 *   - VolumeGroupFactory::the()->registerGroup( this );
	 *
	 * \param QString name of the group
	 * \param int number of channels
	 */
	VolumeGroup( QString, int, QObject* =0, const char* =0 );
	/**
	 * What implementers have to do:
	 *   - Delete the channels
	 */
	virtual ~VolumeGroup();

	/**
	 * returns the masterwidget.
	 * The argument is used the first time for parenting the widget.
	 */
	virtual VolumeGroupMasterWidget* masterWidget( QWidget* ) =0;
	/**
	 * returns a new channelwidget.
	 * \param the name of the input channel
	 * \param The parent for the widget
	 */
	virtual VolumeGroupChannelWidget* channelWidget( QString, QWidget* ) =0;

	int channels() const { return _channels; }
	QString name() const { return _name; }

public slots:
	/**
	 * Is called to remove the VG.
	 */
	void remove();

protected:
	VolumeGroupMasterWidget* _masterwidget;

private:
	QString _name;
	int _channels;
};

class VolumeGroupMasterWidget : public QFrame {
Q_OBJECT
public:
	/**
	 * Creates the needed Layout and sets the group.
	 * It also creates the buttons all groups have ( remove ).
	 *
	 * What implementers have to do:
	 *   - Add the needed widgets to the layout()
	 */
	VolumeGroupMasterWidget( VolumeGroup*, QWidget* =0, const char* =0 );
	/**
	 * Destructor
	 */
	virtual ~VolumeGroupMasterWidget();

	VolumeGroup* group() const { return _group; }

private:
	VolumeGroup* _group;
};

class VolumeGroupChannelWidget : public QFrame {
Q_OBJECT
public:
	/**
	 * Creates the Widget.
	 */
	VolumeGroupChannelWidget( QString, VolumeGroup*, QWidget*, const char* =0 );
	/**
	 * Destructor
	 */
	virtual ~VolumeGroupChannelWidget();

	VolumeGroup* group() const { return _group; }

	void paintEvent( QPaintEvent* );
protected:
	QString inchannel() const { return _in; }

private:
	QString _in;
	VolumeGroup* _group;
};

};

#endif

