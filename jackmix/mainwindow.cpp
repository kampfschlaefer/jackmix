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
#include <qsettings.h>
#include <qfiledialog.h>

#include "defaults.xml"

using namespace JackMix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ), _settings( new QSettings( /*QSettings::Ini*/ ) ) {
	_settings->setPath( "arnoldarts.de", "jackmix", QSettings::User );
	_settings->beginGroup( "jackmix" );
std::cerr << "MainWindow::MainWindow( " << p << ", n )" << std::endl;
	QPopupMenu *file = new QPopupMenu( this );
	menuBar()->insertItem( "File", file );
	file->insertItem( "Open File...", this, SLOT( openFile() ), CTRL+Key_O );
	file->insertItem( "Save File...", this, SLOT( saveFile() ), CTRL+Key_S );
	file->insertItem( "Quit", this, SLOT( close() ), CTRL+Key_Q );

	QPopupMenu *edit = new QPopupMenu( this );
	menuBar()->insertItem( "Edit", edit );
	edit->insertItem( "Add Input...", this, SLOT( addInput() ) );
	edit->insertItem( "Add Output...", this, SLOT( addOutput() ) );

	//new VGAux( "aux", 3, this );
	//new VGStereo( "stereo", this );

	mw = new QHBox( this );
	this->setCentralWidget( mw );
	mw->setSpacing( 3 );

	readXML( _settings->readEntry( "/SaveOnExit/Config", DEFAULTSTRING ) );

	_master = new MasterWidgets( this, "MasterControls" );
	addDockWindow( _master, DockRight );
	connect( VolumeGroupFactory::the(), SIGNAL( sNewVG( VolumeGroup* ) ), _master, SLOT( newVG( VolumeGroup* ) ) );
}

void MainWindow::readXML( QString xml ) {
std::cerr << "MainWindow::init( " << xml << " )" << std::endl;
	QDomDocument doc;
	doc.setContent( xml );
	recursiveXML( doc.documentElement() );
std::cerr << "MainWindow::init() finished..." << std::endl;
}

void MainWindow::recursiveXML( QDomElement elem ) {
	QDomNode tmp = elem.firstChild();
	while ( !tmp.isNull() ) {
		QDomElement elem = tmp.toElement();
		if ( elem.tagName() == "channel" )
			newChannel( new ChannelWidget( elem, mw ) );
		else if ( elem.tagName() == "volumegroup" )
			VGDomCreator::createFromDomElement( elem );
		else recursiveXML( elem );
		tmp = tmp.nextSibling();
	}
}

MainWindow::~MainWindow() {
std::cerr << "MainWindow::~MainWindow()" << std::endl;
}

void MainWindow::closeEvent( QCloseEvent* e ) {
std::cerr << "MainWindow::closeEvent( QCloseEvent " << e << " )" << std::endl;
	_settings->beginGroup( "/SaveOnExit" );
	_settings->writeEntry( "/Config", writeXML() );

	e->accept();
	delete _settings;
}

QString MainWindow::writeXML() {
	QDomDocument doc( "Mainrc" );
	QDomElement docElem = doc.createElement( "Mainrc" );
	doc.appendChild( docElem );

	// In Channels
	QDomElement in = doc.createElement( "in" );
	docElem.appendChild( in );
	for ( uint i = 0; i<_channelwidgets.size(); i++ )
		_channelwidgets[ i ]->appendToDoc( doc, in );

	// Out Channels
	QDomElement out = doc.createElement( "out" );
	docElem.appendChild( out );
	for ( int i = 0; i<VolumeGroupFactory::the()->groups(); i++ )
		VolumeGroupFactory::the()->group( i )->appendToDoc( doc, out );

	qDebug( ">>> XML-tree\n" + doc.toString() + "<<< XML-tree" );

	return doc.toString();
}

void MainWindow::addInput() {
	//std::cerr << "MainWindow::addInput()" << std::endl;
	QString name = QInputDialog::getText( "Name", "Name for the Input" );
	//std::cerr << "MainWindow::addInput() after dialog" << std::endl;
	if ( !name.isEmpty() ) {
		newChannel( new ChannelWidget( name, mw ) );
	}
	//std::cerr << "MainWindow::addInput() finished..." << std::endl;
}
void MainWindow::addOutput() {
	VGSelectDialog *tmp = new VGSelectDialog( 0 );
	if ( tmp->exec() == QDialog::Accepted ) {
		_master->newVG( tmp->newVG() );
	}
}
void MainWindow::removeInput( ChannelWidget* n ) {
	//std::cerr << "MainWindow::removeInput( " << n << " )" << std::endl;
	//std::cerr << ( ( _channelwidgets.remove( n ) )?"true":"false" ) << std::endl;
	_channelwidgets.remove( n );
	delete n;
}

void MainWindow::newChannel( ChannelWidget* n ) {
	_channelwidgets.append( n );
	connect( n, SIGNAL( remove( ChannelWidget* ) ), this, SLOT( removeInput( ChannelWidget* ) ) );
}

void MainWindow::openFile() {
	qDebug( "MainWindow::openFile()" );
	QString file = QFileDialog::getOpenFileName( QString::null, "*.xml", this );
	QString xml;
	if ( file != QString::null ) {
		QFile qfile( file );
		qfile.open( IO_ReadOnly );
		xml = QString( qfile.readAll() );
	}
	//qDebug( xml );
	//qDebug( "About to delete " + QString::number( _channelwidgets.size() ) + " Channels" );
	while ( _channelwidgets.size() >0 )
		_channelwidgets[ 0 ]->remove();
	while ( VolumeGroupFactory::the()->groups()>0 )
		VolumeGroupFactory::the()->group( 0 )->remove();
	//qDebug( "Remaining " + QString::number( _channelwidgets.size() ) + " widgets" );
	_channelwidgets.clear();
	readXML( xml );
}
void MainWindow::saveFile() {
	QString filename = QFileDialog::getSaveFileName( QString::null, "*.xml", this );
	if ( filename != QString::null ) {
		QFile file( filename );
		file.open( IO_WriteOnly );
		QString tmp = writeXML();
		file.writeBlock( tmp.latin1(), tmp.length() );
	}
}

MasterWidgets::MasterWidgets( QWidget* p, const char* n ) : QDockWindow( p,n ) {
	for ( int i=0; i<VolumeGroupFactory::the()->groups(); i++ ) {
		newVG( VolumeGroupFactory::the()->group( i ) );
	}
}
MasterWidgets::~MasterWidgets() {
}

void MasterWidgets::newVG( VolumeGroup* n ) {
	//std::cerr << "MasterWidgets::newVG( " << n << " )" << std::endl;
	QBoxLayout* _layout = boxLayout();
	_layout->addWidget( n->masterWidget( this ) );
	n->masterWidget( this )->show();
}

