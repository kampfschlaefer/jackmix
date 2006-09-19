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
#include <QtGui/QApplication>
#include <QtGui/QSlider>
#include <QtGui/QLabel>

#include "osc_server.h"
#include "osc_connection.h"

int main( int argc, char** argv ) {

	QApplication *qapp = new QApplication( argc, argv );

	OSC::ConnectionServer* _oscconnectionserver = new OSC::ConnectionServer( "5282", qapp );

	OSC::ServerPath *quitaction = _oscconnectionserver->newServerPath( "/quit", QVariant::Invalid, false );
	QObject::connect( quitaction, SIGNAL( data() ), qapp, SLOT( quit() ) );

//	QSlider *slider = new QSlider( 0, 100, 10, 50, Qt::Horizontal, 0 );
//	slider->setDisabled( true );
	( void* ) /*OSC::ServerPath* _slideraction = */_oscconnectionserver->newServerPath( "/slider", QVariant::Int );
//	QObject::connect( _slideraction, SIGNAL( data( int ) ), slider, SLOT( setValue( int ) ) );

//	qapp->setMainWidget( slider );
//	slider->show();

	QWidget* w = new QLabel( "No slider needed anymore...", 0 );
	w->show();

	int ret = qapp->exec();
	delete _oscconnectionserver;
	return ret;
}
