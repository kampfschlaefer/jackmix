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

#include "editablechannelselector.h"
#include "editablechannelselector.moc"

#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>

#include <QtCore/qdebug.h>

namespace JackMix {
namespace GUI {

EditableChannelSelector::EditableChannelSelector( QString title
						, QString label
						, QStringList channels
						, QWidget *p
						, const char* n )
    : ChannelSelector(title, label, channels, p, n)
    , _channels(channels) {
	// Just like a ChannelSelector, but the user can edit the channel names.
	_list->setEditTriggers(QAbstractItemView::AllEditTriggers);
	_items = channels.length();
	_isEdited = new bool[_items];
	for (int i = 0; i < _items; i++) {
		_list->item(i)->setFlags(_list->item(i)->flags() | Qt::ItemIsEditable);
		_isEdited[i] = false;
	}
	connect(_list, SIGNAL(itemChanged(QListWidgetItem*)),
		this, SLOT(register_changed_item(QListWidgetItem*)) );
}

EditableChannelSelector::~EditableChannelSelector() {
	delete[] _isEdited;
}

/// A public method allowing an external agent to update the
/// list content after choosing an alternative name to avoid duplications
void EditableChannelSelector::update_channel_name(int which, QString revised) {
	//qDebug() << "EditableChannelSelector::update_channel_name(" << which
	//         << ", " << revised << "): _channels =" << _channels;
	_channels[which] = revised;
	_list->item(which)->setText(revised);
	_isEdited[which] = false;
}
/// Handle user edits of list contents
void EditableChannelSelector::register_changed_item(QListWidgetItem *wi) {
	//qDebug() << "Item changed: " << _list->row(wi);
	int which = _list->row(wi);
	_isEdited[which] = true;
}

void EditableChannelSelector::commit() {
	qDebug("EditableChannelSelector");
	QStringList changed;
	for (int c = 0; c < _items; c++)
		if (_isEdited[c]) {
			changed.append(_channels[c]);
			changed.append(_channels[c] = _list->item(c)->text());
			_isEdited[c] = false;
		}
	
	emit editedChannels(changed);
}


}
}
