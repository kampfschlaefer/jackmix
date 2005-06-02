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
#include <qslider.h>

//#include "textedit.h"
#include "osc_server.h"

int main( int argc, char** argv ) {
	//std::cout << "JackMix-Server starting" << std::endl;

	QApplication *qapp = new QApplication( argc, argv );

	OSC::Server* _oscserver = new OSC::Server();

	OSC::ServerPath *quitaction = new OSC::ServerPath( _oscserver, "/quit", QVariant::Invalid );
	QObject::connect( quitaction, SIGNAL( data() ), qapp, SLOT( quit() ) );

//	TextEdit *mw = new TextEdit();
//	const char* signal = SIGNAL( gotData( QString, QVariant ) );
//	const char* slot = SLOT( appendData( QString, QVariant ) );
//	qDebug( "! %s -> %s ? %s", signal, slot, ( QObject::connect( _oscserver, signal, mw, slot ) )?"True":"False" );
//	mw->show();

//	qapp->setMainWidget( mw );

	QSlider *slider = new QSlider( 0, 100, 10, 50, Qt::Horizontal, 0 );
	slider->setDisabled( true );
	OSC::ServerPath* _slideraction = new OSC::ServerPath( _oscserver, "/slider", QVariant::Int );
	QObject::connect( _slideraction, SIGNAL( data( int ) ), slider, SLOT( setValue( int ) ) );

	qapp->setMainWidget( slider );
	slider->show();

	int ret = qapp->exec();
	delete _oscserver;
	return ret;
}
