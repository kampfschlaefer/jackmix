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

#include "lineinput.h"
#include "osc_client.h"

int main( int argc, char** argv ) {
//	std::cout << "JackMix-Server starting" << std::endl;

	QString host="localhost", port="7770";
	for ( int i=0; i<argc; i++ ) {
		qDebug( "argv[%i] : %s", i, argv[ i ] );
		if ( QString( argv[ i ] ) == "-h" && argc > i )
			host = argv[ i+1 ];
		if ( QString( argv[ i ] ) == "-p" && argc > i )
			port = argv[ i+1 ];
	}
	QApplication *qapp = new QApplication( argc, argv );

	//JackMix::OSC::Client* _client = JackMix::OSC::Client::the();

	LineInput *mw = new LineInput();
	OSC::Client* _client = new OSC::Client( host, port, mw );

	_client->sendData( "/test/string", "Halli" );
	_client->sendData( "/test/string", QString( "Galli" ) );
	_client->sendData( "/test/double", 0.005 );
	_client->sendData( "/test/int", 42 );

	mw->connect( mw, SIGNAL( textChanged( QString ) ), _client, SLOT( sendData( QString ) ) );
	mw->show();

	qapp->setMainWidget( mw );

	int ret = qapp->exec();
	return ret;
}
