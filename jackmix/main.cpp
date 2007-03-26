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

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtGui/QApplication>

#include "jack_backend.h"
#include "mainwindow.h"

int main( int argc, char** argv ) {
	qDebug() << "JackMix starting";

	QApplication *qapp = new QApplication( argc, argv );
	QStringList args = qapp->arguments();

	QString file;
	for( int i=1; i<args.size(); ++i ) {
		qDebug() << QString( " arg %1: %2" ).arg( i ).arg( args[ i ] );
		if ( QFile::exists( args[ i ] ) )
			file = args[ i ];
	}

	JackMix::MainWindow *mw;
	if ( !file.isEmpty() )
		mw = new JackMix::MainWindow( file );
	else
		mw = new JackMix::MainWindow();
	mw->show();

	int ret = qapp->exec();

	delete mw;

	return ret;
}
