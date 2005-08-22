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
#include <qdatetime.h>

#include "lineinput.h"
#include "osc_client.h"
#include "osc_server.h"
#include "osc_connection.h"
#include <updatefilter.h>

int main( int argc, char** argv ) {
//	srand( QTime::currentTime().msec() );

	QString host="localhost", port="7770";
	for ( int i=0; i<argc; i++ ) {
		qDebug( "argv[%i] : %s", i, argv[ i ] );
		if ( QString( argv[ i ] ) == "-h" && argc > i )
			host = argv[ i+1 ];
		if ( QString( argv[ i ] ) == "-p" && argc > i )
			port = argv[ i+1 ];
	}
	QApplication *qapp = new QApplication( argc, argv );

	OSC::ConnectionClient* _connectionclient = new OSC::ConnectionClient( host, port, qapp );

	QVBox *mw = new QVBox();
	LineInput *_lineinput = new LineInput( mw );

	_lineinput->connect( _lineinput, SIGNAL( textChanged( QString ) ), _connectionclient->client(), SLOT( sendData( QString ) ) );

	QSlider* slider = new QSlider( 0, 100, 10, 50, Qt::Horizontal, mw );
	UpdateFilter* sliderfilter = new UpdateFilter( slider );
	QObject::connect( slider, SIGNAL( valueChanged( int ) ), sliderfilter, SLOT( dataInInternal( int ) ) );
	QObject::connect( sliderfilter, SIGNAL( dataOutInternal( int ) ), slider, SLOT( setValue( int ) ) );
	OSC::ClientPath* slideraction = _connectionclient->newClientPath( "/slider", QVariant::Int );
	QObject::connect( sliderfilter, SIGNAL( dataOut( int ) ), slideraction, SLOT( data( int ) ) );
	OSC::ServerPath* sliderupdate = _connectionclient->newServerPath( "/slider", QVariant::Int );
	QObject::connect( sliderupdate, SIGNAL( data( int ) ), sliderfilter, SLOT( dataIn( int ) ) );
	mw->show();

	qapp->setMainWidget( mw );

	int ret = qapp->exec();
	delete _connectionclient;
	return ret;
}
