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

#ifndef JACK_CHANNELWIDGET_H
#define JACK_CHANNELWIDGET_H

#include <qframe.h>
#include <qstring.h>
#include <qptrlist.h>
#include <qdom.h>

namespace JackMix {

class VolumeGroup;
class VolumeGroupChannelWidget;

class ChannelWidget : public QFrame {
Q_OBJECT
public:
	ChannelWidget( QString name, QWidget*, const char* =0 );
	ChannelWidget( QDomElement, QWidget*, const char* =0 );
	~ChannelWidget();

	QDomElement toXML();
public slots:
	void newVG( VolumeGroup* );
	void removeVG( VolumeGroup* );
private slots:
	void valueChanged( QString, float );
	void remove();
private:
	void init();
	QString _name;
	QPtrList<VolumeGroupChannelWidget> _groupwidgets;
};

};

#endif

