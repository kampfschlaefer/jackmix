
#include "mainwindow.h"
#include "mainwindow.moc"

#include <qpopupmenu.h>
#include <qmenubar.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );
}
MainWindow::~MainWindow() {
}

