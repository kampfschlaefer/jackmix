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

#ifndef JACKMIX_STEREOVOLUMESLIDER_H
#define JACKMIX_STEREOVOLUMESLIDER_H

#include <qframe.h>
#include <qstring.h>
#include "common.h"
#include "dbvolcalc.h"

namespace JackMix {

class StereoVolumeSlider : public QFrame, public dB2VolCalc {
Q_OBJECT
public:
	/**
		\param QString channel1
		\param QString channel2
		\param float dbmin
		\param float dbmax
	*/
	StereoVolumeSlider( QString, QString, float, float, QWidget*, const char* =0 );
	~StereoVolumeSlider();
signals:
	void valueChanged( QString, float );
private slots:
	void balanceChanged( float );
	void volumeChanged( float );
private:
	void updateVolumes();
	float _balance, _volume;
	QString _channel1, _channel2;
};

};

#endif

