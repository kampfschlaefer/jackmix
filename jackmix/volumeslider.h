
#ifndef JACK_VOLUMESLIDER_H
#define JACK_VOLUMESLIDER_H

#include <qframe.h>
#include <qstring.h>
#include "common.h"
#include "dbvolcalc.h"

namespace JackMix {

class VolumeSlider : public QFrame, public dB2VolCalc {
Q_OBJECT
public:
	/**
		\param QString name of the Channel
		\param float initial volume value
	*/
	VolumeSlider( QString, float, Direction, QWidget* =0, const char* =0 );
	~VolumeSlider();
signals:
	void valueChanged( QString, float );
private slots:
	void iValueChanged( float );
private:
	QString _name;
};

};

#endif

