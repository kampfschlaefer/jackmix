/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

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
#include <qinputdialog.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );

	QPopupMenu *edit = new QPopupMenu( this );
	menuBar()->insertItem( "Edit", edit );
	edit->insertItem( "Add Input", this, SLOT( addInput() ) );

	void* tmp;
	tmp = new VGAux( "aux", 3, this );
	tmp = new VGStereo( "stereo", this );

	mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 3 );
	tmp = new ChannelWidget( "in_1", mw );
	tmp = new ChannelWidget( "in_2", mw );
//	tmp = new ChannelWidget( "in_3", mw );
//	tmp = new ChannelWidget( "in_4", mw );

	addDockWindow( new MasterWidgets( this, "MasterControls" ), DockRight );
}
MainWindow::~MainWindow() {
}

void MainWindow::addInput() {
	void* tmp;
	QString name = QInputDialog::getText( "Name", "Name for the Input" );
	if ( !name.isEmpty() )
		tmp = new ChannelWidget( name, mw );
}
void MainWindow::addOutput() {

}

MasterWidgets::MasterWidgets( QWidget* p, const char* n ) : QDockWindow( p,n ) {
	QBoxLayout* _layout = boxLayout();//new QVBoxLayout( this );
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		_layout->addWidget( VolumeGroupFactory::the()->group( i )->masterWidget( this ) );
	}
}
MasterWidgets::~MasterWidgets() {
}

