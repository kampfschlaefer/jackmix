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

#ifndef JACKMIX_MAINWINDOW_H
#define JACKMIX_MAINWINDOW_H

#include <qmainwindow.h>
#include <qdockwindow.h>
#include <qvaluelist.h>

class QHBox;
class QSettings;
class QDomElement;
class QVBox;

namespace JackMix {

class ChannelWidget;
class VolumeGroup;
class MasterWidgets;

class MainWindow : public QMainWindow {
Q_OBJECT
public:
	MainWindow( QWidget* =0, const char* =0 );
	~MainWindow();
public slots:
	void addInput();
	void addOutput();
	void removeInput( ChannelWidget* );
	void openFile();
	void saveFile();
private slots:
	void readXML( QString );
	QString writeXML();
	void closeEvent( QCloseEvent* );
	void toggleRestore();
	void about();
	void aboutQt();
private:
	void recursiveXML( QDomElement );
	void newChannel( ChannelWidget* );
	QHBox* mw;
	QValueList<ChannelWidget*> _channelwidgets;
	MasterWidgets* _master;
	QSettings* _settings;

	int config_restore_id;
	QPopupMenu *_filemenu, *_editmenu, *_settingsmenu, *_helpmenu;
};

class MasterWidgets : public QDockWindow {
Q_OBJECT
public:
	MasterWidgets( QWidget* =0, const char* =0 );
	~MasterWidgets();
public slots:
	void newVG( VolumeGroup* );
private:
	QVBox* _layout;
};

};

#endif
