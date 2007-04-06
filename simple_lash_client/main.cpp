
#include <QtGui/QtGui>

#include <lash/lash.h>
#include <qlash.h>

#include "main.h"

int main( int argc, char** argv ) {
	qDebug() << "simple_lash_client starting";

	QApplication qapp( argc, argv );

	qLash::qLashClient* c = new qLash::qLashClient( "LASH QPushButton", argc, argv );

	QObject::connect( c, SIGNAL( quitApp() ), &qapp, SLOT( closeAllWindows() ) );

	MyButton* btn = new MyButton( "Button" );
	btn->setCheckable( true );
	QObject::connect( btn, SIGNAL( state( int ) ), c, SLOT( saveToConfig( int ) ) );
	QObject::connect( c, SIGNAL( saveToConfig() ), btn, SLOT( signalState() ) );

	btn->show();

	int ret = qapp.exec();

	delete c;

	qDebug() << "simple_lash_client finishing with return-code" << ret;
	return ret;
}

