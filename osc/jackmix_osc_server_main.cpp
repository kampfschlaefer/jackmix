/*
    Copyright ( C ) 2005 Arnold Krille <arnold@arnoldarts.de>

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

#include <iostream>
#include <qapplication.h>
#include <qtextedit.h>

#include "jack_backend.h"

#include "oscserver.h"

int main( int argc, char** argv ) {
	std::cout << "JackMix-Server starting" << std::endl;

	QApplication *qapp = new QApplication( argc, argv );

	(void*) JackMix::JackBackend::backend();
	JackMix::OSC::Server* _oscserver = new JackMix::OSC::Server();

	QTextEdit *mw = new QTextEdit();
	mw->setTextFormat( Qt::LogText );
	QObject::connect( _oscserver, SIGNAL( gotData( QString ) ), mw, SLOT( append( const QString & ) ) );
	mw->show();

	qapp->setMainWidget( mw );

	int ret = qapp->exec();
	delete _oscserver;
	(void*) JackMix::JackBackend::backend( true );
	return ret;
}
