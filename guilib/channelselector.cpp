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

#include "channelselector.h"
#include "channelselector.moc"

#include <qstring.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>

ChannelSelector::ChannelSelector( QString title, QString label, QStringList channels, QWidget* p, const char* n ) : QDialog( p,n ) {
	this->setCaption( title );

	QGridLayout *_layout = new QGridLayout( this, 4,4, 5,5 );
	QLabel *_label = new QLabel( label, this );
	_layout->addMultiCellWidget( _label, 0,0, 0,3 );

	_list = new QListBox( this );
	_list->insertStringList( channels );
	_layout->addMultiCellWidget( _list, 1,1, 0,3 );

	_layout->addMultiCell( new QSpacerItem( 10,10 ), 2,2, 0,3 );

	_layout->addItem( new QSpacerItem( 40,10 ), 3,0 );
	_commit_n_quit = new QPushButton( "Commit && Quit", this );
	_commit_n_quit->setDefault( true );
	connect( _commit_n_quit, SIGNAL( clicked() ), this, SLOT( commitnquit() ) );
	_layout->addWidget( _commit_n_quit, 3,3 );
	_commit = new QPushButton( "Commit", this );
	connect( _commit, SIGNAL( clicked() ), this, SLOT( commit() ) );
	_layout->addWidget( _commit, 3,2 );
	_cancel = new QPushButton( "Cancel", this );
	connect( _cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	_layout->addWidget( _cancel, 3,1 );

}
ChannelSelector::~ChannelSelector() {
}

void ChannelSelector::commit() {
qDebug( "ChannelSelector::commit()" );
qDebug( "Returning: %s", _list->currentText().latin1() );
	emit selectedChannel( _list->currentText() );
}
void ChannelSelector::commitnquit() {
qDebug( "ChannelSelector::commitnquit()" );
	commit();
qDebug( "Now quit..." );
	done( 0 );
}

void ChannelSelector::addChannel( QString ) {
}
void ChannelSelector::removeChannel( QString ) {
}

