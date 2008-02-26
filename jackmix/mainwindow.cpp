/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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
#include "graphicalguiserver.h"

#include "aux_elements.h"
#include "stereo_elements.h"

#include "qlash.h"

#include <QtCore/QDebug>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QLayout>
#include <QtGui/QInputDialog>
#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtGui/QAction>
#include <QtCore/QTimer>
#include <QtGui/QCloseEvent>
#include <QtGui/QStatusBar>
#include <QtGui/QApplication>

#include <QtXml/QDomDocument>


using namespace JackMix;
using namespace JackMix::MixingMatrix;

MainWindow::MainWindow( QWidget* p ) : QMainWindow( p ), _backend( new JackBackend( new GUI::GraphicalGuiServer( this ) ) ), _autofillscheduled( true ) {
	qDebug() << "MainWindow::MainWindow(" << p << ")";
	init();

	QStringList ins;
	QStringList outs;
	{
		QStringList args = qApp->arguments();
		bool yes = false;
		foreach( QString tmp, args )
			if ( tmp.contains( "lash" ) )
				yes = true;
		if ( !yes ) {
			ins = QStringList() << "in_1" << "in_2" << "in_3" << "in_4" << "in_5" << "in_6" << "in_7" << "in_8";
			outs = QStringList() << "out_1" << "out_2";
		}
	}

	foreach( QString in, ins )
		addInput( in );
	foreach( QString out, outs )
		addOutput( out );

	ins = _backend->inchannels();
	outs = _backend->outchannels();
	if ( ins.empty() || outs.empty() )
		statusBar()->showMessage( "No Channels available :-(" );

	_autofillscheduled = false;
	scheduleAutoFill();

	qDebug() << "MainWindow::MainWindow() finished...";
}
MainWindow::MainWindow( QString filename, QWidget* p ) : QMainWindow( p ), _backend( new JackBackend( new GUI::GraphicalGuiServer( this ) ) ), _autofillscheduled( true ) {
	qDebug() << "MainWindow::MainWindow(" << filename << "," << p << ")";
	init();

	openFile( filename );

	QStringList ins = _backend->inchannels();
	QStringList outs = _backend->outchannels();
	if ( ins.empty() || outs.empty() )
		statusBar()->showMessage( "No Channels available :-(" );

	_autofillscheduled = false;
	scheduleAutoFill();

	//_lashclient->setJackName( "JackMix" );

	qDebug() << "MainWindow::MainWindow() finished...";
}

void MainWindow::init() {

	JackMix::MixerElements::init_aux_elements();
	JackMix::MixerElements::init_stereo_elements();

	_filemenu = menuBar()->addMenu( "&File" );
	_filemenu->addAction( "Open File...", this, SLOT( openFile() ), Qt::CTRL+Qt::Key_O );
	_filemenu->addAction( "Save File...", this, SLOT( saveFile() ), Qt::CTRL+Qt::Key_S );
	_filemenu->addSeparator();
	_filemenu->addAction( "&Quit", this, SLOT( close() ), Qt::CTRL+Qt::Key_Q );

	_editmenu = menuBar()->addMenu( "&Edit" );
	_select_action = new QAction( "Select Mode", this );
	_select_action->setCheckable( true );
	connect( _select_action, SIGNAL( triggered() ), this, SLOT( toggleselectmode() ) );
	//_editmenu->addAction( _select_action );
	//_select_action->addTo( new QToolBar( this ) );
	_editmenu->addAction( "&Fill empty spaces", this, SLOT( scheduleAutoFill() ) );
	_editmenu->addSeparator();
	_add_inchannel_action = new QAction( "Add &Input...", this );
	connect( _add_inchannel_action, SIGNAL( triggered() ), this, SLOT( addInput() ) );
	_editmenu->addAction( _add_inchannel_action );
	_add_outchannel_action = new QAction( "Add &Output...", this );
	connect( _add_outchannel_action, SIGNAL( triggered() ), this, SLOT( addOutput() ) );
	_editmenu->addAction( _add_outchannel_action );
	_remove_inchannel_action = new QAction( "&Remove Input...", this );
	connect( _remove_inchannel_action, SIGNAL( triggered() ), this, SLOT( removeInput() ) );
	_editmenu->addAction( _remove_inchannel_action );
	_remove_outchannel_action = new QAction( "R&emove Output...", this );
	connect( _remove_outchannel_action, SIGNAL( triggered() ), this, SLOT( removeOutput() ) );
	_editmenu->addAction( _remove_outchannel_action );

	_viewmenu = menuBar()->addMenu( "&View" );
	_togglein_action = new QAction( "Hide &inputcontrols", this );
	connect( _togglein_action, SIGNAL( triggered() ), this, SLOT( togglein() ) );
	_viewmenu->addAction( _togglein_action );
	_toggleout_action = new QAction( "Hide &outputcontrols", this );
	connect( _toggleout_action, SIGNAL( triggered() ), this, SLOT( toggleout() ) );
	_viewmenu->addAction( _toggleout_action );

	_helpmenu = menuBar()->addMenu( "&Help" );
	_helpmenu->addAction( "About &JackMix", this, SLOT( about() ) );
	_helpmenu->addAction( "About &Qt", this, SLOT( aboutQt() ) );

	_mw = new MainWindowHelperWidget( this );
	setCentralWidget( _mw );

	_mixerwidget = new MixingMatrix::Widget( QStringList() << "i1", QStringList() << "o1", _backend, _mw );
	_mixerwidget->removeinchannel( "i1" );
	_mixerwidget->removeoutchannel( "o1" );
	_mw->layout->addWidget( _mixerwidget, 1,0 );
	_inputswidget = new MixingMatrix::Widget( QStringList() << "i1", QStringList(), _backend, _mw );
	_inputswidget->removeinchannel( "i1" );
	_mw->layout->addWidget( _inputswidget, 0,0 );
	_outputswidget = new MixingMatrix::Widget( QStringList(), QStringList() << "o1", _backend, _mw );
	_outputswidget->removeoutchannel( "o1" );
	_mw->layout->addWidget( _outputswidget, 1,1 );

	_mw->layout->setRowStretch( 0, 1 );
	_mw->layout->setRowStretch( 1, int( 1E2 ) );
	_mw->layout->setColumnStretch( 1, 1 );
	_mw->layout->setColumnStretch( 0, int( 1E2 ) );

//	_debugPrint = new QAction( "DebugPrint", CTRL+Key_P, this );
//	connect( _debugPrint, SIGNAL( activated() ), _mixerwidget, SLOT( debugPrint() ) );
//	_debugPrint->addTo( _filemenu );

	_select_action->toggle();
	toggleselectmode();

	_lashclient = new qLash::qLashClient( "JackMix", 0,0, this );
	connect( _lashclient, SIGNAL( quitApp() ), this, SLOT( close() ) );
	connect( _lashclient, SIGNAL( saveToDir( QString ) ), this, SLOT( saveLash( QString ) ) );
	connect( _lashclient, SIGNAL( restoreFromDir( QString ) ), this, SLOT( restoreLash( QString ) ) );
	//_lashclient->setJackName( "JackMix" );
}

MainWindow::~MainWindow() {
	qDebug() << "MainWindow::~MainWindow()";
	delete _backend;
}

void MainWindow::closeEvent( QCloseEvent* e ) {
	qDebug() << "MainWindow::closeEvent( QCloseEvent " << e << " )";
	e->accept();
}

void MainWindow::openFile() {
	QString path = QFileDialog::getOpenFileName( this, 0, 0, "JackMix-XML (*.jm-xml)" );
	openFile( path );
}

void MainWindow::openFile( QString path ) {
	//qDebug() << "MainWindow::openFile(" << path << ")";
	if ( path.isEmpty() )
		return;

	// delay autofill at least until all saved elements are created:
	bool save_autofillscheduled = _autofillscheduled;
	_autofillscheduled = true;

	QFile file( path );
	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		while ( _backend->outchannels().size() > 0 )
			removeOutput( _backend->outchannels()[ 0 ] );
		while ( _backend->inchannels().size() > 0 )
			removeInput( _backend->inchannels()[ 0 ] );

		QDomDocument doc( "bla" );
		doc.setContent( &file );

		QDomElement jackmix = doc.documentElement();
		QString version = jackmix.attribute( "version", "0.3" );

		if ( version == "0.3" ) {

			QDomElement ins = jackmix.firstChildElement( "ins" );
			for ( QDomElement in = ins.firstChildElement( "channel" ); !in.isNull(); in = in.nextSiblingElement( "channel" ) ) {
				QString name = in.attribute( "name" );
				QString volume = in.attribute( "volume" );
				addInput( name );
				_backend->setVolume( name, name, volume.toDouble() );
			}

			QDomElement outs = jackmix.firstChildElement( "outs" );
			for ( QDomElement out = outs.firstChildElement( "channel" ); !out.isNull(); out = out.nextSiblingElement( "channel" ) ) {
				QString name = out.attribute( "name" );
				QString volume = out.attribute( "volume" );
				addOutput( name );
				_backend->setVolume( name, name, volume.toDouble() );
			}

			QDomElement matrix = jackmix.firstChildElement( "matrix" );
			for ( QDomElement volume = matrix.firstChildElement( "volume" ); !volume.isNull(); volume = volume.nextSiblingElement( "volume" ) ) {
				QString in = volume.attribute( "in" );
				QString out = volume.attribute( "out" );
				QString value = volume.attribute( "value" );
				_backend->setVolume( in, out, value.toDouble() );
			}

			QDomElement elements = jackmix.firstChildElement( "elements" );
			for ( QDomElement element = elements.firstChildElement( "element" ); !element.isNull(); element = element.nextSiblingElement( "element" ) ) {
				QStringList ins;
				for ( QDomElement in = element.firstChildElement( "in" ); !in.isNull(); in = in.nextSiblingElement( "in" ) )
					ins << in.attribute( "name" );
				QStringList outs;
				for ( QDomElement out = element.firstChildElement( "out" ); !out.isNull(); out = out.nextSiblingElement( "out" ) )
					outs << out.attribute( "name" );
				_mixerwidget->createControl( ins, outs );
			}
		}

		file.close();
	}

	_autofillscheduled = save_autofillscheduled;
	scheduleAutoFill();
	//qDebug() << "MainWindow::openFile() finished";
}
void MainWindow::saveFile( QString path ) {
	if ( path.isEmpty() )
		path = QFileDialog::getSaveFileName( this, 0, 0, "JackMix-XML (*.jm-xml)" );
	if ( path.isEmpty() )
		return;
	
	if ( ! path.endsWith( ".jm-xml" ) )
		path += ".jm-xml";

	QStringList ins = _backend->inchannels();
	QStringList outs = _backend->outchannels();

	QString xml = "<jackmix version=\"0.3\"><ins>";
	foreach( QString in, ins ) {
		xml += QString( "<channel name=\"%1\" volume=\"%2\" />" ).arg( in ).arg( _backend->getVolume( in,in ) );
	}
	xml += "</ins><outs>";
	foreach( QString out, outs )
		xml += QString( "<channel name=\"%1\" volume=\"%2\" />" ).arg( out ).arg( _backend->getVolume( out,out ) );
	xml += "</outs><matrix>";
	foreach( QString in, ins )
		foreach( QString out, outs )
			xml += QString( "<volume in=\"%1\" out=\"%2\" value=\"%3\" />" ).arg( in ).arg( out ).arg( _backend->getVolume( in, out ) );
	xml += "</matrix>";

	xml += "<elements>";
	// Elements
	QList<Element*> savedelements;
	foreach( QString in, ins )
		foreach( QString out, outs ) {
			Element* tmp = _mixerwidget->getResponsible( in, out );
			if ( !savedelements.contains( tmp ) && ( tmp->in().size() > 1 || tmp->out().size() > 1 ) ) {
				xml += "<element>";
				foreach( QString input, tmp->in() )
					xml += QString( "<in name=\"%1\" />" ).arg( input );
				foreach( QString output, tmp->out() )
					xml += QString( "<out name=\"%1\" />" ).arg( output );
				xml += "</element>";
				savedelements.push_back( tmp );
			}
		}
	xml += "</elements>";

	xml += "</jackmix>";

	QFile file( path );
	if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		QTextStream stream( &file );
		stream << xml.replace( ">", ">\n" );
		file.close();
	}

}

void MainWindow::about() {
	QMessageBox::about( this, "JackMix: About JackMix", "<qt> \
		<p><b>&copy;2004-2007 by Arnold Krille</b> &lt;arnold@arnoldarts.de&gt;</p> \
		<p>JackMix is the ultimative mixer application for Jack (<a href=\"http://www.jackaudio.org/\">www.jackaudio.org</a>). Check out <a href=\"http://www.arnoldarts.de/drupal/?q=JackMix\">http://www.arnoldarts.de/drupal/?q=JackMix</a> for more information and new versions of JackMix.</p> \
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
		_inputswidget->addinchannel( name );
		scheduleAutoFill();
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
		_outputswidget->addoutchannel( name );
		scheduleAutoFill();
	}
}

void MainWindow::removeInput() {
	//qDebug( "MainWindow::removeInput()" );
	JackMix::GUI::ChannelSelector *tmp = new JackMix::GUI::ChannelSelector( "Delete Inputchannels", "Select the inputchannels for deletion:", _backend->inchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeInput( QString ) ) );
	tmp->show();
}
void MainWindow::removeInput( QString n ) {
	//qDebug( "MainWindow::removeInput( QString %s )", qPrintable( n ) );
	if ( _backend->removeInput( n ) ) {
		_inputswidget->removeinchannel( n );
		_mixerwidget->removeinchannel( n );
		scheduleAutoFill();
	}
}
void MainWindow::removeOutput() {
	//qDebug( "MainWindow::removeOutput()" );
	JackMix::GUI::ChannelSelector *tmp = new JackMix::GUI::ChannelSelector( "Delete Outputchannels", "Select the outputchannels for deletion:", _backend->outchannels(), this );
	connect( tmp, SIGNAL( selectedChannel( QString ) ), this, SLOT( removeOutput( QString ) ) );
	tmp->show();
}
void MainWindow::removeOutput( QString n ) {
	//qDebug( "MainWindow::removeOutput( QString %s )", qPrintable( n ) );
	if ( _backend->removeOutput( n ) ) {
		_outputswidget->removeoutchannel( n );
		_mixerwidget->removeoutchannel( n );
		scheduleAutoFill();
	}
}

void MainWindow::allAutoFill() {
	_mixerwidget->autoFill();
	_outputswidget->autoFill();
	_inputswidget->autoFill();
	_autofillscheduled = false;
}
void MainWindow::scheduleAutoFill() {
	if ( !_autofillscheduled ) {
		QTimer::singleShot( 1, this, SLOT( allAutoFill() ) );
		_autofillscheduled = true;
	}
}

void MainWindow::saveLash( QString dir ) {
	qDebug() << "MainWindow::saveLash(" << dir << ")";
	QString file = QString( "%1/jackmix.jm-xml" ).arg( dir );
	saveFile( file );
}
void MainWindow::restoreLash( QString dir ) {
	qDebug() << "MainWindow::restoreLash(" << dir << ")";
	QString file = QString( "%1/jackmix.jm-xml" ).arg( dir );
	openFile( file );
	_lashclient->setJackName( "JackMix" );
	qDebug() << "MainWindow::restoreLash() finished";
}


MainWindowHelperWidget::MainWindowHelperWidget( QWidget* p ) : QWidget( p ) {
	layout = new QGridLayout( this );
	layout->setMargin( 2 );
	layout->setSpacing( 2 );
}

