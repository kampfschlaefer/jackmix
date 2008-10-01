
#include <QtGui/QtGui>

#include "mixingwidget2.h"

using namespace JackMix::MixingMatrix;

int main( int argc, char** argv )
{

	QApplication* app = new QApplication( argc, argv );

	Widget2* w = new Widget2( 0 );
	w->show();

	w->addInputChannel( "in1" );
	w->addInputChannel( "in2" );
	w->addInputChannel( "in3" );
	w->addOutPutChannel( "out1" );
	w->addOutPutChannel( "out2" );

	return app->exec();
}

