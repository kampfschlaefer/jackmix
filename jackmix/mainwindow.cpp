
#include "mainwindow.h"
#include "mainwindow.moc"

#include "channelwidget.h"
#include "jack_backend.h"

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qhbox.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );

	BACKEND->addOutput( "out_L" );
	BACKEND->addOutput( "out_R" );

	QHBox *mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 5 );
	void* tmp = new ChannelWidget( "in_1", mw );
	tmp = ( void* ) new ChannelWidget( "in_2", mw );
	tmp = ( void* ) new ChannelWidget( "in_3", mw );
	tmp = ( void* ) new ChannelWidget( "in_4", mw );
}
MainWindow::~MainWindow() {
}

