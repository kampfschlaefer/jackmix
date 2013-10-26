/*
    Copyright 2013 Nick Bailey <nick@n-ism.org>

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

#ifndef EDITABLECHANNELSELECTOR_H
#define EDITABLECHANNELSELECTOR_H

#include "channelselector.h"

class QListWidgetItem;

namespace JackMix {
namespace GUI {

class EditableChannelSelector : public ChannelSelector {
	Q_OBJECT
public:
	EditableChannelSelector(QString title, QString label, QStringList channels, QWidget*, const char* =0);
	virtual ~EditableChannelSelector();
signals:
	/**
	 * A EditableChannelSelector emits a list of channel names which have changed.
	 * The list is of the form:
	 *   from_1, to_1, from_2, to_2...
	 */
	void editedChannels( QStringList );

public slots:
	/** If the channel name's already in use, an external agency may choose an alternative
	 *  on behalf of the user, for example by appending _1, _2 etc.
	 * \param which The index of the entry to change
	 * \param revised The modified unique name
	 */
	virtual void update_channel_name(int which, QString revised);

private slots:
	virtual void register_changed_item(QListWidgetItem *wi);
	virtual void commit();
private:
	bool *_isEdited;
	int _items;
	QStringList _channels;
};


}
}

#endif
