/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

    Modified to permit control via MIDI by Nick Bailey <nick@n-ism.org>
    Released as version 0.5.0 (et seq.) copyright 2013.

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

#define JACKMIX_FILE_FORMAT_VERSION	"0.5"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtWidgets/QGridLayout>
#include <mixingmatrix.h>
#include "controlsender.h"

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

signals:
	/** If the user requests a channel name already in use,
	 *  an alternative is chosen and this signal is fired
	 */
	void modify_channel_name(int index, QString replacement);

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

	void renameInput();
	void renameInput( QStringList );
	void renameOutput();
	void renameOutput( QStringList );

	void removeInput();
	void removeInput( QString );
	void removeOutput();
	void removeOutput( QString );

	//void slot_simple_replace();

	void allAutoFill();
	void scheduleAutoFill();

	void about();
	void aboutQt();

	void saveLash( QString dir );
	void restoreLash( QString dir );

	/** Set all of the contained elements' MIDI parameters only after AutoFill is over */
	void updateAutoFilledMidiParams(MixingMatrix::Widget *);

private:
	void init();
	int config_restore_id;
	QMenu *_filemenu, *_editmenu, *_viewmenu, *_settingsmenu, *_helpmenu;
	MixingMatrix::Widget *_mixerwidget, *_inputswidget, *_outputswidget;
	/** Elements in the main window's widgets should be allowed to complete their
	 *  initialisation before their MIDI parameters are set. The following lists of
	 *  midi parameters are applied to its respective widget after initialisation
	 *  is complete.
	 */
	QHash<QString,QString> _mixermps, _inputmps, _outputmps;
	MainWindowHelperWidget* _mw;
	QAction *_select_action
	      , *_togglein_action, *_toggleout_action
	      , *_add_inchannel_action, *_add_outchannel_action
	      , *_rename_input_action, *_rename_output_action
	      ,*_remove_inchannel_action, *_remove_outchannel_action;
	QAction *_debugPrint;
	BackendInterface* _backend;
	bool _autofillscheduled;

	qLash::qLashClient* _lashclient;

        MidiControl::ControlSender* midiControlSender;
};

};
#endif
