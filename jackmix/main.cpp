
#include <iostream>
#include <qapplication.h>

#include "jack_backend.h"
#include "mainwindow.h"

int main( int argc, char** argv ) {
	std::cout << "JackMix starting" << std::endl;

	QApplication *qapp = new QApplication( argc, argv );

	/*JackMix::JackBackend* backend =*/ (void*) JackMix::JackBackend::backend();

	JackMix::MainWindow *mw = new JackMix::MainWindow();
	mw->show();

	qapp->setMainWidget( mw );

	int ret = qapp->exec();
	//delete backend;
	return ret;
}
