
#include "mainwindow.h"
#include "mainwindow.moc"

#include "channelwidget.h"
#include "jack_backend.h"
#include "volumegroup.h"
#include "vg_aux.h"
#include "vg_stereo.h"

#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qhbox.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );

	void* tmp;
	//tmp = new VolumeGroup( "out", 2, this );
	tmp = new VGAux( "aux", 5, this );
	tmp = new VGStereo( "stereo", this );

	QHBox *mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 3 );
	tmp = new ChannelWidget( "in_1", mw );
	tmp = new ChannelWidget( "in_2", mw );
	tmp = new ChannelWidget( "in_3", mw );
	tmp = new ChannelWidget( "in_4", mw );
}
MainWindow::~MainWindow() {
}

