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
#include "vgselect.h"

#include <iostream>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qinputdialog.h>

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
std::cerr << "MainWindow::MainWindow( " << p << ", n )" << std::endl;
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Quit", this, SLOT( close() ), Key_Q );

	QPopupMenu *edit = new QPopupMenu( this );
	menuBar()->insertItem( "Edit", edit );
	edit->insertItem( "Add Input", this, SLOT( addInput() ) );
	edit->insertItem( "Add Output", this, SLOT( addOutput() ) );

std::cerr << "MainWindow::MainWindow() VolumeGroups" << std::endl;
	new VGAux( "aux", 3, this );
	new VGStereo( "stereo", this );

std::cerr << "MainWindow::MainWindow() Layout" << std::endl;
	mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 3 );

	_master = new MasterWidgets( this, "MasterControls" );
	addDockWindow( _master, DockRight );
	init();
}

void MainWindow::init() {
std::cerr << "MainWindow::init()" << std::endl;
	newChannel( new ChannelWidget( "in_1", mw ) );
	newChannel( new ChannelWidget( "in_2", mw ) );
	newChannel( new ChannelWidget( "in_3", mw ) );
	newChannel( new ChannelWidget( "in_4", mw ) );
std::cerr << "MainWindow::init() finished..." << std::endl;
}
MainWindow::~MainWindow() {
}

void MainWindow::addInput() {
std::cerr << "MainWindow::addInput()" << std::endl;
	QString name = QInputDialog::getText( "Name", "Name for the Input" );
	//std::cerr << "MainWindow::addInput() after dialog" << std::endl;
	if ( !name.isEmpty() ) {
		newChannel( new ChannelWidget( name, mw ) );
//		_channelwidgets.append( tmp );
//		connect( tmp, SIGNAL( remove( ChannelWidget* ) ), this, SLOT( removeInput( ChannelWidget* ) ) );
	}
	//std::cerr << "MainWindow::addInput() finished..." << std::endl;
}
void MainWindow::addOutput() {
	VGSelectDialog *tmp = new VGSelectDialog( 0 );
	if ( tmp->exec() == QDialog::Accepted ) {
		for ( uint i=0; i<_channelwidgets.count(); i++ ) {
			_channelwidgets[ i ]->newVG( tmp->newVG() );
		}
	}
	_master->newVG( tmp->newVG() );
}
void MainWindow::removeInput( ChannelWidget* n ) {
std::cerr << "MainWindow::removeInput( " << n << " )" << std::endl;
	std::cerr << ( ( _channelwidgets.remove( n ) )?"true":"false" ) << std::endl;
	delete n;
}

void MainWindow::newChannel( ChannelWidget* n ) {
	_channelwidgets.append( n );
	connect( n, SIGNAL( remove( ChannelWidget* ) ), this, SLOT( removeInput( ChannelWidget* ) ) );
}


MasterWidgets::MasterWidgets( QWidget* p, const char* n ) : QDockWindow( p,n ) {
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		newVG( VolumeGroupFactory::the()->group( i ) );
	}
}
MasterWidgets::~MasterWidgets() {
}

void MasterWidgets::newVG( VolumeGroup* n ) {
std::cerr << "MasterWidgets::newVG( " << n << " )" << std::endl;
	QBoxLayout* _layout = boxLayout();
	_layout->addWidget( n->masterWidget( this ) );
	n->masterWidget( this )->show();
}

