
#include <qapplication.h>
#include "qfloatcontrol.h"

using namespace JackMix::GUI;

int main( int argc, char** argv ) {
	QApplication *app = new QApplication( argc, argv );

	QFloatControl* tmp = new QFloatControl( 0 );;
	tmp->show();

	app->setMainWidget( tmp );

	return app->exec();
}

