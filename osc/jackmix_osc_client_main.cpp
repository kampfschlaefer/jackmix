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
#include <qvbox.h>
#include <qslider.h>

#include "lineinput.h"
#include "osc_client.h"
//#include "osc_server.h"

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

	QVBox *mw = new QVBox();
	LineInput *_lineinput = new LineInput( mw );
	OSC::Client* _client = new OSC::Client( host, port, _lineinput );
//	OSC::Server* _server = new OSC::Server( QString::number( rand() ), _lineinput );
//	qDebug( "My address is \"%s\"", _server->url().latin1() );

//	_client->sendData( "/registerclient", _server->url() );
	_client->sendData( "/test/string", "Halli" );
	_client->sendData( "/test/string", QString( "Galli" ) );
	_client->sendData( "/test/double", 0.005 );
	_client->sendData( "/test/int", 42 );

	_lineinput->connect( _lineinput, SIGNAL( textChanged( QString ) ), _client, SLOT( sendData( QString ) ) );

	QSlider* slider = new QSlider( 0, 100, 10, 50, Qt::Horizontal, mw );
	OSC::ClientPath* slideraction = new OSC::ClientPath( _client, "/slider", QVariant::Int );
	QObject::connect( slider, SIGNAL( valueChanged( int ) ), slideraction, SLOT( data( int ) ) );
	mw->show();

	qapp->setMainWidget( mw );

	int ret = qapp->exec();
	return ret;
}
