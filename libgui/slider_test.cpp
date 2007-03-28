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

#include <iostream>
#include <QtGui/QApplication>
#include <QtGui/QColor>

#include "slider.h"
#include "knob.h"

int main( int argc, char** argv ) {
	QApplication *qapp = new QApplication( argc, argv );

	QWidget *mw = new JackMix::GUI::Slider( 0, -20, 3, 2, 1, 0 );
	//QWidget *mw = new JackMix::GUI::Knob( 0, -20, 3, 2, 1, 0 );
	mw->show();

	int ret = qapp->exec();
	return ret;
}
