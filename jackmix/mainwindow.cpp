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
#include "channelselector.h"

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

//#include "defaults.xml"

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
	_editmenu->insertItem( "Autofill", this, SLOT( autofill() ) );
	_editmenu->insertSeparator();
	_add_inchannel_action = new QAction( "Add Input...", 0, this );
	connect( _add_inchannel_action, SIGNAL( activated() ), this, SLOT( addInput() ) );
	_add_inchannel_action->addTo( _editmenu );
	_add_outchannel_action = new QAction( "Add Output...", 0, this );
	connect( _add_outchannel_action, SIGNAL( activated() ), this, SLOT( addOutput() ) );
	_add_outchannel_action->addTo( _editmenu );
	_remove_inchannel_action = new QAction( "Remove Input...", 0, this );
	connect( _remove_inchannel_action, SIGNAL( activated() ), this, SLOT( removeInput() ) );
	_remove_inchannel_action->addTo( _editmenu );
	_remove_outchannel_action = new QAction( "Remove Output...", 0, this );
	connect( _remove_outchannel_action, SIGNAL( activated() ), this, SLOT( removeOutput() ) );
	_remove_outchannel_action->addTo( _editmenu );

	_viewmenu = new QPopupMenu( this );
	menuBar()->insertItem( "View", _viewmenu );
	_togglein_action = new QAction( "Hide inputcontrols", 0, this );
	connect( _togglein_action, SIGNAL( activated() ), this, SLOT( togglein() ) );
	_togglein_action->addTo( _viewmenu );
	_toggleout_action = new QAction( "Hide outputcontrols", 0, this );
	connect( _toggleout_action, SIGNAL( activated() ), this, SLOT( toggleout() ) );
	_toggleout_action->addTo( _viewmenu );
	_showLister = new QAction( "Toggle ConnectionLister", CTRL+Key_L, this );
	_showLister->addTo( _viewmenu );

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
	_mixerwidget->autoFill();

	//_inputswidget->createControl( QStringList()<<"in_1"<<"in_2", QStringList()<<"in_1"<<"in_2" );
	//_inputswidget->createControl( QStringList()<<"in_3"<<"in_4", QStringList()<<"in_3"<<"in_4" );
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

	_debugPrint = new QAction( "DebugPrint", CTRL+Key_P, this );
	connect( _debugPrint, SIGNAL( activated() ), _mixerwidget, SLOT( debugPrint() ) );
	_debugPrint->addTo( _filemenu );

	connect( _showLister, SIGNAL( activated() ), _mixerwidget, SLOT( toggleConnectionLister() ) );
}

MainWindow::~MainWindow() {
std::cerr << "MainWindow::~MainWindow()" << std::endl;
}

void MainWindow::closeEvent( QCloseEvent* e ) {
std::cerr << "MainWindow::closeEvent( QCloseEvent " << e << " )" << std::endl;
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
void MainWindow::togglein() {
	bool shown = _inputswidget->isShown();
	if ( shown ) {
		_inputswidget->hide();
		_togglein_action->setMenuText( "Show inputcontrols" );
	} else {
		_inputswidget->show();
		_togglein_action->setMenuText( "Hide inputcontrols" );
	}
}
void MainWindow::toggleout() {
	bool shown = _outputswidget->isShown();
	if ( shown ) {
		_outputswidget->hide();
		_toggleout_action->setMenuText( "Show outputcontrols" );
	} else {
		_outputswidget->show();
		_toggleout_action->setMenuText( "Hide outputcontrols" );
	}
}

void MainWindow::addInput() {
	QString tmp = QInputDialog::getText( "Inchannel name", "Channel name", QLineEdit::Normal, "(empty)", 0, this );
	if ( tmp != "(empty)" )
		addInput( tmp );
}
void MainWindow::addInput( QString name ) {
	BACKEND->addInput( name );
	_mixerwidget->addinchannel( name );
	_mixerwidget->autoFill();
	_inputswidget->addinchannel( name );
	_inputswidget->autoFill();
}
void MainWindow::addOutput() {
	QString tmp = QInputDialog::getText( "Outchannel name", "Channel name", QLineEdit::Normal, "(empty)", 0, this );
	if ( tmp != "(empty)" )
		addOutput( tmp );
}
void MainWindow::addOutput( QString name ) {
	BACKEND->addOutput( name );
	_mixerwidget->addoutchannel( name );
	_mixerwidget->autoFill();
	_outputswidget->addoutchannel( name );
	_outputswidget->autoFill();
}

void MainWindow::removeInput() {
qDebug( "MainWindow::removeInput()" );
	ChannelSelector *tmp = new ChannelSelector( "Delete Inputchannels", "Selecte the inputchannels for deletion:", BACKEND->inchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeInput( QString ) ) );
	tmp->show();
}
void MainWindow::removeInput( QString n ) {
qDebug( "MainWindow::removeInput( QString %s )", n.latin1() );
}
void MainWindow::removeOutput() {
qDebug( "MainWindow::removeOutput()" );
	ChannelSelector *tmp = new ChannelSelector( "Delete Outputchannels", "Selecte the outputchannels for deletion:", BACKEND->outchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeOutput( QString ) ) );
	tmp->show();
}
void MainWindow::removeOutput( QString n ) {
qDebug( "MainWindow::removeOutput( QString %s )", n.latin1() );
}

void MainWindow::autofill() {
	_mixerwidget->autoFill();
}


MainWindowHelperWidget::MainWindowHelperWidget( QWidget* p ) : QWidget( p ) {
	layout = new QGridLayout( this, 3,2, 5 );
}

