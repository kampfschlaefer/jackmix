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

#include "jack_backend.h"
#include "mixingmatrix.h"
#include "aux_elements.h"

#include <iostream>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qinputdialog.h>
#include <qsettings.h>
#include <qfiledialog.h>
#include <qscrollview.h>
#include <qmessagebox.h>
#include <qaction.h>

#include "defaults.xml"

using namespace JackMix;
using namespace MixingMatrix;

MainWindow::MainWindow( QWidget* p, const char* n ) : QMainWindow( p,n ) {
std::cerr << "MainWindow::MainWindow( " << p << ", n )" << std::endl;
	_filemenu = new QPopupMenu( this );
	menuBar()->insertItem( "File", _filemenu );
	//_filemenu->insertItem( "Open File...", this, SLOT( openFile() ), CTRL+Key_O );
	//_filemenu->insertItem( "Save File...", this, SLOT( saveFile() ), CTRL+Key_S );
	//_filemenu->insertSeparator();
	_filemenu->insertItem( "Quit", this, SLOT( close() ), CTRL+Key_Q );

	_editmenu = new QPopupMenu( this );
	menuBar()->insertItem( "Edit", _editmenu );
	_select_action = new QAction( "Select Mode", 0, this );
	_select_action->setToggleAction( true );
	connect( _select_action, SIGNAL( activated() ), this, SLOT( toggleselectmode() ) );
	_select_action->addTo( _editmenu );
	//_select_action->addTo( new QToolBar( this ) );
	_editmenu->setCheckable( true );
	//_editmenu->insertItem( "Add Input...", this, SLOT( addInput() ) );
	//_editmenu->insertItem( "Add Output...", this, SLOT( addOutput() ) );

	_helpmenu = new QPopupMenu( this );
	menuBar()->insertItem( "Help", _helpmenu );
	_helpmenu->insertItem( "About JackMix", this, SLOT( about() ) );
	_helpmenu->insertItem( "About Qt", this, SLOT( aboutQt() ) );

	_mw = new MainWindowHelperWidget( this );	
	setCentralWidget( _mw );
	_mw->layout->setRowStretch( 0, 0 );
	_mw->layout->setRowStretch( 1, 1000 );
	_mw->layout->setColStretch( 1, 0 );
	_mw->layout->setColStretch( 0, 1000 );

	QStringList ins = QStringList()<<"in_1"<<"in_2"<<"in_3"<<"in_4"<<"in_5"<<"in_6"<<"in_7"<<"in_8";
	QStringList outs = QStringList()<<"out_1"<<"out_2"<<"out_3";
	_mixerwidget = new MixingMatrix::Widget( ins, outs, _mw );
	_mw->layout->addWidget( _mixerwidget, 1,0 );
	_inputswidget = new MixingMatrix::Widget( ins, QStringList(), _mw );
	_mw->layout->addWidget( _inputswidget, 0,0 );
	_outputswidget = new MixingMatrix::Widget( QStringList(), outs, _mw );
	_mw->layout->addWidget( _outputswidget, 1,1 );

	_mixerwidget->createControl( QStringList()<<"in_1"<<"in_2", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_3"<<"in_4", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_5", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_6", QStringList()<<"out_1"<<"out_2" );
	//_mixerwidget->createControl( QStringList()<<"in_4", QStringList()<<"out_1" );
	//_mixerwidget->createControl( QStringList()<<"in_4", QStringList()<<"out_2" );
	_mixerwidget->autoFill();

	_inputswidget->createControl( QStringList()<<"in_1"<<"in_2", QStringList()<<"in_1"<<"in_2" );
	_inputswidget->createControl( QStringList()<<"in_3"<<"in_4", QStringList()<<"in_3"<<"in_4" );
	_inputswidget->autoFill();

	//_outputswidget->createControl( QStringList()<<"out_1"<<"out_2", QStringList()<<"out_1"<<"out_2" );
	_outputswidget->autoFill();

	BACKEND->addInput( "in_1" );
	BACKEND->addInput( "in_2" );
	BACKEND->addInput( "in_3" );
	BACKEND->addInput( "in_4" );
	BACKEND->addInput( "in_5" );
	BACKEND->addInput( "in_6" );
	BACKEND->addInput( "in_7" );
	BACKEND->addInput( "in_8" );
	BACKEND->addOutput( "out_1" );
	BACKEND->addOutput( "out_2" );
	BACKEND->addOutput( "out_3" );
}

MainWindow::~MainWindow() {
std::cerr << "MainWindow::~MainWindow()" << std::endl;
}

void MainWindow::closeEvent( QCloseEvent* e ) {
std::cerr << "MainWindow::closeEvent( QCloseEvent " << e << " )" << std::endl;
	delete _mixerwidget;
	e->accept();
}

void MainWindow::openFile() {
}
void MainWindow::saveFile() {
}

void MainWindow::about() {
	QMessageBox::about( this, "JackMix: About JackMix", "<qt> \
		<p><b>&copy;2004 by Arnold Krille</b> &lt;arnold@arnoldarts.de&gt;</p> \
		<p>JackMix is the ultimative mixer application for Jack (<a href=\"http://jackit.sf.net/\">jackit.sf.net</a>). Check out <a href=\"http://roederberg.dyndns.org/~arnold/jackmix/\">roederberg.dyndns.org/~arnold/jackmix/</a> for more information and new versions of JackMix.</p> \
		<p>This application and all its components are licensed under the GPL.</p> \
		</qt>" );
}
void MainWindow::aboutQt() {
	QMessageBox::aboutQt( this, "JackMix: About Qt" );
}

void MainWindow::toggleselectmode() {
	bool select = ( _mixerwidget->mode() == Widget::Select );
	if ( select )
		_mixerwidget->mode( Widget::Normal );
	else
		_mixerwidget->mode( Widget::Select );
	_select_action->setOn( !select );
}

MainWindowHelperWidget::MainWindowHelperWidget( QWidget* p ) : QWidget( p ) {
	layout = new QGridLayout( this, 2,2, 5 );
}

