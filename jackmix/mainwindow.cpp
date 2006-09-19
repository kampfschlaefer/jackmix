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
#include <QtCore/QDebug>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
//#include <QtGui/qhbox>
//#include <QtGui/qvbox>
#include <QtGui/QLayout>
#include <QtGui/QInputDialog>
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
//#include <QtGui/QScrollArea>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtCore/QTimer>
#include <QtGui/QCloseEvent>

//#include "defaults.xml"

using namespace JackMix;
using namespace JackMix::MixingMatrix;

MainWindow::MainWindow( QWidget* p ) : QMainWindow( p ), _backend( new JackBackend() ) {
std::cerr << "MainWindow::MainWindow( " << p << ", n )" << std::endl;
	_filemenu = new QMenu( this );
	menuBar()->addMenu( _filemenu );
	//_filemenu->insertItem( "Open File...", this, SLOT( openFile() ), CTRL+Key_O );
	//_filemenu->insertItem( "Save File...", this, SLOT( saveFile() ), CTRL+Key_S );
	//_filemenu->insertSeparator();
	_filemenu->addAction( "Quit", this, SLOT( close() ), Qt::CTRL+Qt::Key_Q );

	_editmenu = new QMenu( this );
	menuBar()->addMenu( _editmenu );
	_select_action = new QAction( "Select Mode", this );
	_select_action->setCheckable( true );
	connect( _select_action, SIGNAL( triggered() ), this, SLOT( toggleselectmode() ) );
	_editmenu->addAction( _select_action );
	//_select_action->addTo( new QToolBar( this ) );
	//_editmenu->setCheckable( true );
	_editmenu->addAction( "Fill empty spaces", this, SLOT( autofill() ) );
	_editmenu->addSeparator();
	_add_inchannel_action = new QAction( "Add Input...", this );
	connect( _add_inchannel_action, SIGNAL( triggered() ), this, SLOT( addInput() ) );
	_editmenu->addAction( _add_inchannel_action );
	_add_outchannel_action = new QAction( "Add Output...", this );
	connect( _add_outchannel_action, SIGNAL( triggered() ), this, SLOT( addOutput() ) );
	_editmenu->addAction( _add_outchannel_action );
	_remove_inchannel_action = new QAction( "Remove Input...", this );
	connect( _remove_inchannel_action, SIGNAL( triggered() ), this, SLOT( removeInput() ) );
	_editmenu->addAction( _remove_inchannel_action );
	_remove_outchannel_action = new QAction( "Remove Output...", this );
	connect( _remove_outchannel_action, SIGNAL( triggered() ), this, SLOT( removeOutput() ) );
	_editmenu->addAction( _remove_outchannel_action );

	_viewmenu = new QMenu( this );
	menuBar()->addMenu( _viewmenu );
	_togglein_action = new QAction( "Hide inputcontrols", this );
	connect( _togglein_action, SIGNAL( triggered() ), this, SLOT( togglein() ) );
	_viewmenu->addAction( _togglein_action );
	_toggleout_action = new QAction( "Hide outputcontrols", this );
	connect( _toggleout_action, SIGNAL( triggered() ), this, SLOT( toggleout() ) );
	_viewmenu->addAction( _toggleout_action );
	_showLister = new QAction( "Toggle ConnectionLister", this );
	_showLister->setShortcut( Qt::CTRL + Qt::Key_L );
	_viewmenu->addAction( _showLister );

	_helpmenu = new QMenu( this );
	menuBar()->addMenu( _helpmenu );
	_helpmenu->addAction( "About JackMix", this, SLOT( about() ) );
	_helpmenu->addAction( "About Qt", this, SLOT( aboutQt() ) );

	_mw = new MainWindowHelperWidget( this );	
	setCentralWidget( _mw );
	//_mw->layout->setRowStretch( 0, 0 );
	//_mw->layout->setRowStretch( 1, 1000 );
	//_mw->layout->setColStretch( 1, 0 );
	//_mw->layout->setColStretch( 0, 1000 );

	QStringList ins = QStringList();//<<"in_1"<<"in_2"<<"in_3"<<"in_4"<<"in_5"<<"in_6"<<"in_7"<<"in_8";
	QStringList outs = QStringList();//<<"out_1"<<"out_2"<<"out_3";
	_mixerwidget = new MixingMatrix::Widget( ins, outs, _backend, _mw );
	_mw->layout->addWidget( _mixerwidget, 1,0 );
	_inputswidget = new MixingMatrix::Widget( ins, QStringList(), _backend, _mw );
	_mw->layout->addWidget( _inputswidget, 0,0 );
	_outputswidget = new MixingMatrix::Widget( QStringList(), outs, _backend, _mw );
	_mw->layout->addWidget( _outputswidget, 1,1 );

	/*_mixerwidget->createControl( QStringList()<<"in_1"<<"in_2", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_3"<<"in_4", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_5", QStringList()<<"out_1"<<"out_2" );
	_mixerwidget->createControl( QStringList()<<"in_6", QStringList()<<"out_1"<<"out_2" );*/
	//_mixerwidget->autoFill();
	QTimer::singleShot( 1, this, SLOT( autofill() ) );

	//_inputswidget->createControl( QStringList()<<"in_1"<<"in_2", QStringList()<<"in_1"<<"in_2" );
	//_inputswidget->createControl( QStringList()<<"in_3"<<"in_4", QStringList()<<"in_3"<<"in_4" );
	_inputswidget->autoFill();

	//_outputswidget->createControl( QStringList()<<"out_1"<<"out_2", QStringList()<<"out_1"<<"out_2" );
	_outputswidget->autoFill();

	_backend->addInput( "in_1" );
	_backend->addInput( "in_2" );
	_backend->addInput( "in_3" );
	_backend->addInput( "in_4" );
	_backend->addInput( "in_5" );
	_backend->addInput( "in_6" );
	_backend->addInput( "in_7" );
	_backend->addInput( "in_8" );
	_backend->addOutput( "out_1" );
	_backend->addOutput( "out_2" );
	_backend->addOutput( "out_3" );

//	_debugPrint = new QAction( "DebugPrint", CTRL+Key_P, this );
//	connect( _debugPrint, SIGNAL( activated() ), _mixerwidget, SLOT( debugPrint() ) );
//	_debugPrint->addTo( _filemenu );

	connect( _showLister, SIGNAL( activated() ), _mixerwidget, SLOT( toggleConnectionLister() ) );
	_select_action->toggle();
	toggleselectmode();

	qDebug() << "MainWindow::MainWindow() finished...";
}

MainWindow::~MainWindow() {
	std::cerr << "MainWindow::~MainWindow()" << std::endl;
	delete _backend;
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
	_select_action->setChecked( !select );
}
void MainWindow::togglein() {
	bool shown = _inputswidget->isVisible();
	if ( shown ) {
		_inputswidget->hide();
		_togglein_action->setText( "Show inputcontrols" );
	} else {
		_inputswidget->show();
		_togglein_action->setText( "Hide inputcontrols" );
	}
}
void MainWindow::toggleout() {
	bool shown = _outputswidget->isVisible();
	if ( shown ) {
		_outputswidget->hide();
		_toggleout_action->setText( "Show outputcontrols" );
	} else {
		_outputswidget->show();
		_toggleout_action->setText( "Hide outputcontrols" );
	}
}

void MainWindow::addInput() {
	QString tmp = QInputDialog::getText( this, "Inchannel name", "Channel name", QLineEdit::Normal, "(empty)" );
	if ( tmp != "(empty)" )
		addInput( tmp );
}
void MainWindow::addInput( QString name ) {
	if ( _backend->addInput( name ) ) {
		_mixerwidget->addinchannel( name );
		_mixerwidget->autoFill();
		_inputswidget->addinchannel( name );
		_inputswidget->autoFill();
	}
}
void MainWindow::addOutput() {
	QString tmp = QInputDialog::getText( this, "Outchannel name", "Channel name", QLineEdit::Normal, "(empty)" );
	if ( tmp != "(empty)" )
		addOutput( tmp );
}
void MainWindow::addOutput( QString name ) {
	if ( _backend->addOutput( name ) ) {
		_mixerwidget->addoutchannel( name );
		_mixerwidget->autoFill();
		_outputswidget->addoutchannel( name );
		_outputswidget->autoFill();
	}
}

void MainWindow::removeInput() {
qDebug( "MainWindow::removeInput()" );
	ChannelSelector *tmp = new ChannelSelector( "Delete Inputchannels", "Select the inputchannels for deletion:", _backend->inchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeInput( QString ) ) );
	tmp->show();
}
void MainWindow::removeInput( QString n ) {
	qDebug( "MainWindow::removeInput( QString %s )", n.toStdString().c_str() );
	if ( _backend->removeInput( n ) ) {
		_inputswidget->removeinchannel( n );
		_mixerwidget->removeinchannel( n );
	}
}
void MainWindow::removeOutput() {
qDebug( "MainWindow::removeOutput()" );
	ChannelSelector *tmp = new ChannelSelector( "Delete Outputchannels", "Select the outputchannels for deletion:", _backend->outchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeOutput( QString ) ) );
	tmp->show();
}
void MainWindow::removeOutput( QString n ) {
qDebug( "MainWindow::removeOutput( QString %s )", n.toStdString().c_str() );
	if ( _backend->removeOutput( n ) ) {
		_outputswidget->removeoutchannel( n );
		_mixerwidget->removeoutchannel( n );
	}
}

void MainWindow::autofill() {
	_mixerwidget->autoFill();
}


MainWindowHelperWidget::MainWindowHelperWidget( QWidget* p ) : QWidget( p ) {
	layout = new QGridLayout( this );
}

