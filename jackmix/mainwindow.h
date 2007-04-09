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

#ifndef JACKMIX_MAINWINDOW_H
#define JACKMIX_MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>
#include <QtCore/QList>
#include <QtGui/QGridLayout>

class QHBox;
class QSettings;
class QDomElement;
class QVBox;
class QAction;
class QMenu;

namespace qLash {
	class qLashClient;
};

namespace JackMix {

class BackendInterface;

namespace MixingMatrix {
	class Widget;
}

class MainWindowHelperWidget : public QWidget {
Q_OBJECT
public:
	MainWindowHelperWidget( QWidget* =0 );
	QGridLayout* layout;
	QBoxLayout* in_layout;
};

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow( QWidget* =0 );
	MainWindow( QString filename, QWidget* =0 );
	~MainWindow();
public slots:
	void openFile();
	void openFile( QString path );
	void saveFile( QString path="" );
private slots:
	void toggleselectmode();
	void togglein();
	void toggleout();
	void closeEvent( QCloseEvent* );

	void addInput();
	void addInput( QString );
	void addOutput();
	void addOutput( QString );

	void removeInput();
	void removeInput( QString );
	void removeOutput();
	void removeOutput( QString );

	void allAutoFill();
	void scheduleAutoFill();

	void about();
	void aboutQt();

	void saveLash( QString dir );
	void restoreLash( QString dir );

private:
	void init();
	int config_restore_id;
	QMenu *_filemenu, *_editmenu, *_viewmenu, *_settingsmenu, *_helpmenu;
	MixingMatrix::Widget *_mixerwidget, *_inputswidget, *_outputswidget;
	MainWindowHelperWidget* _mw;
	QAction *_select_action, *_togglein_action, *_toggleout_action, *_add_inchannel_action, *_add_outchannel_action, *_remove_inchannel_action, *_remove_outchannel_action;
	QAction *_debugPrint;
	BackendInterface* _backend;
	bool _autofillscheduled;

	qLash::qLashClient* _lashclient;
};

};
#endif
