
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
#include <qlayout.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );

	void* tmp;
	tmp = new VGAux( "aux", 3, this );
	tmp = new VGStereo( "stereo", this );

	QHBox *mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 3 );
	tmp = new ChannelWidget( "in_1", mw );
	tmp = new ChannelWidget( "in_2", mw );
	tmp = new ChannelWidget( "in_3", mw );
	tmp = new ChannelWidget( "in_4", mw );

	addDockWindow( new MasterWidgets( this, "MasterControls" ), DockRight );
}
MainWindow::~MainWindow() {
}

MasterWidgets::MasterWidgets( QWidget* p, const char* n ) : QDockWindow( p,n ) {
	QBoxLayout* _layout = boxLayout();//new QVBoxLayout( this );
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		_layout->addWidget( VolumeGroupFactory::the()->group( i )->masterWidget( this ) );
	}
}
MasterWidgets::~MasterWidgets() {
}

