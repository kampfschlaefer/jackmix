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

#ifndef JACKMIX_VOLUMEGROUP_AUX_H
#define JACKMIX_VOLUMEGROUP_AUX_H

#include "volumegroup.h"
#include <qvaluevector.h>

namespace JackMix {

class VGAuxChannelWidget;
class VGAuxMasterWidget;

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
	VolumeGroupMasterWidget* masterWidget( QWidget* );
	/**
		returns a new channelwidget.
		\param the name of the input channel
		\param The parent for the widget
	*/
	VolumeGroupChannelWidget* channelWidget( QString, QWidget* );
public:
	void removeVG();
private:
	QValueVector<VGAuxChannelWidget*> _channelwidgets;
};

};

#endif

