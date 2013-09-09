#ifndef MIDICONTROLCHANNELASSIGNER_TEST_H
#define MIDICONTROLCHANNELASSIGNER_TEST_H

#include <QtGui/QWidget>
#include <QtCore/QVector>
#include <QtCore/QtDebug>

#include "midicontrolchannelassigner.h"

class ResultPrinter : public QWidget {
Q_OBJECT
public:
	ResultPrinter(JackMix::GUI::MidiControlChannelAssigner* emitter) {
		connect(emitter, SIGNAL(assignChannels(QVector<int>)),
			this, SLOT(showCtrlValues(QVector<int>)) );
	};

	virtual ~ResultPrinter() { };
	
private slots:
	void showCtrlValues(QVector<int> v) {
		for (int i=0; i < v.size(); i++)
			qDebug() << "Control " << i << ": " << v[i];
	};
};

#endif
