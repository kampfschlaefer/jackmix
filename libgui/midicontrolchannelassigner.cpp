/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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

#include "midicontrolchannelassigner.h"
#include "midicontrolchannelassigner.moc"

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

namespace JackMix {
namespace GUI {

MidiControlChannelAssigner::MidiControlChannelAssigner( QString title, QString label, QStringList controls, const QList<int> &init, QWidget* p )
	: QDialog( p ) {
	this->setWindowTitle( title );

	QGridLayout *_layout = new QGridLayout( this );
	QLabel *_label = new QLabel( label, this );
	_layout->addWidget( _label, 0,0, 1,4 );
	_num_controls = controls.size();
	// reserve space for control variables. When the user's satisfied, we'll send a signal.
	_cchans = new QSpinBox*[_num_controls];
	// MIDI supports 120 control channels.
	// Channels 120 and above are reserved for "Channel Mode Messages"
	// See http://www.midi.org/techspecs/midimessages.php
	for (int i = 0; i < _num_controls; i++ ) {
		_cchans[i] = new QSpinBox(this);
		_cchans[i]->setRange(0, 119);
		int ival;
		if (i>= init.size() || init[i] < 0)
			ival = 0;
		else if (init[i] >= 119)
			ival = 119;
		else
			ival = init[i];
		
		_cchans[i]->setValue(ival);
	}
	
	int layout_row = 1;
	for (int i = 0; i < _num_controls; i++) {
		QLabel *ctrl_name = new QLabel(controls[i], this);
		_layout->addWidget( ctrl_name, layout_row, 0, 1, 3, Qt::AlignRight);
		_layout->addWidget( _cchans[i], layout_row, 3);
		layout_row++;
	}
	
	_layout->addItem( new QSpacerItem( 10,10 ), layout_row++,0, 1,4 );
	_layout->addItem( new QSpacerItem( 40,10 ), layout_row,0 );
	_commit_n_quit = new QPushButton( "Commit && Quit", this );
	_commit_n_quit->setDefault( true );
	connect( _commit_n_quit, SIGNAL( clicked() ), this, SLOT( commitnquit() ) );
	_layout->addWidget( _commit_n_quit, layout_row,3 );
	_commit = new QPushButton( "Commit", this );
	connect( _commit, SIGNAL( clicked() ), this, SLOT( commit() ) );
	_layout->addWidget( _commit, layout_row,2 );
	_cancel = new QPushButton( "Cancel", this );
	connect( _cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
	_layout->addWidget( _cancel, layout_row,1 );
}

MidiControlChannelAssigner::~MidiControlChannelAssigner() {
}

void MidiControlChannelAssigner::commit() {
	//qDebug( "MidiControlChannelAssigner::commit()" );
	QList<int> values;
	for (int i=0; i<_num_controls; i++)
		values.append(_cchans[i]->value());
	emit assignParameters( values );
}
void MidiControlChannelAssigner::commitnquit() {
	//qDebug( "MidiControlChannelAssigner::commitnquit()" );
	commit();
	//qDebug( "Now quit..." );
	done( 0 );
}

};
};

