/*
    Copyright 2013 Nick Bailey <nick@n-ism.org>


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

#ifndef MIDICONTROLCHANNELASSIGNER_TEST_H
#define MIDICONTROLCHANNELASSIGNER_TEST_H

#include <QtGui/QWidget>
#include <QtCore/QList>
#include <QtCore/QtDebug>

#include "midicontrolchannelassigner.h"

class ResultPrinter : public QWidget {
Q_OBJECT
public:
	ResultPrinter(JackMix::GUI::MidiControlChannelAssigner* emitter) {
		connect(emitter, SIGNAL(assignParameters(QList<int>)),
			this, SLOT(showParameterValues(QList<int>)) );
	};

	virtual ~ResultPrinter() { };
	
private slots:
	void showParameterValues(QList<int> v) {
		for (int i=0; i < v.size(); i++)
			qDebug() << "Parameter " << i << ": " << v[i];
	};
};

#endif
