/*
    Copyright 2004 - 2006 Arnold Krille <arnold@arnoldarts.de>

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

#include <QtGui/QApplication>
#include <QtCore/QtDebug>

#include "midicontrolchannelassigner.h"
#include "midicontrolchannelassigner_test.h"
#include "midicontrolchannelassigner_test.moc"

int main( int argc, char** argv ) {
	QApplication *qapp = new QApplication( argc, argv );

	JackMix::GUI::MidiControlChannelAssigner *mw
		= new JackMix::GUI::MidiControlChannelAssigner( "Test",
							        "Test Channel Allocation",
							        QStringList()<<"Channel 1"<<"Channel 2",
							        QList<int>() << 32 << 33
							      );
	
	ResultPrinter rp(mw);
	mw->show();

	int ret = qapp->exec();
	return ret;
}
