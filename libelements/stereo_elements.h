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

#ifndef STEREO_ELEMENTS_H
#define STEREO_ELEMENTS_H

#include <mixingmatrix.h>
#include <dbvolcalc.h>
#include <controlreceiver.h>

namespace JackMix {

namespace GUI {
	class Slider;
	class Knob;
	class MidiControlChannelAssigner;
}

namespace MixerElements {

class Mono2StereoElement;
class Stereo2StereoElement;
class Mono2StereoElement2;
/**
 * A MonotoStereo control.
 */
class Mono2StereoElement : public JackMix::MixingMatrix::Element , public dB2VolCalc
{
Q_OBJECT
Q_PROPERTY( double volume READ volume WRITE volume );
Q_PROPERTY( double panorama READ panorama WRITE set_panorama );
public:
	Mono2StereoElement( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~Mono2StereoElement();

	int inchannels() const { return 1; }
	int outchannels() const { return 2; }

	double volume() const { return _volume_value; }
	double panorama() const { return _balance_value; }
signals:
	void volume_changed( double );
	void panorama_changed( double );
public slots:
	void set_panorama( double n ) { balance( n ); }
	void balance( double );
	void volume( double );
private slots:
	void calculateVolumes();
private:
	QString _inchannel, _outchannel1, _outchannel2;
	JackMix::GUI::Knob *_balance;
	JackMix::GUI::Slider *_volume;
	double _balance_value, _volume_value;
};

/**
 * A StereotoStereo control.
 */
class Stereo2StereoElement : public JackMix::MixingMatrix::Element, public dB2VolCalc
{
Q_OBJECT
Q_PROPERTY( double balance READ getBalance WRITE balance )
Q_PROPERTY( double volume READ getVolume WRITE volume )
public:
	Stereo2StereoElement( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~Stereo2StereoElement();

	int inchannels() const { return 2; }
	int outchannels() const { return 2; }

	double getVolume() const { return _volume_value; }
	double getBalance() const { return _balance_value; }
private slots:
	void balance( double );
	void volume( double );
	void calculateVolumes();
private:
	QString _inchannel1, _inchannel2, _outchannel1, _outchannel2;
	double _balance_value, _volume_value;
	JackMix::GUI::Slider *_volume_widget, *_balance_widget;
};


class Mono2StereoElement2 : public JackMix::MixingMatrix::Element , public dB2VolCalc
{
Q_OBJECT
Q_PROPERTY( double volume READ volume WRITE volume );
Q_PROPERTY( double panorama READ panorama WRITE set_panorama );
//Q_PROPERTY( double volume2 READ volume2 WRITE volume2 );
Q_PROPERTY( double panorama2 READ panorama2 WRITE set_panorama2 );
//Q_PROPERTY( double volume3 READ volume3 WRITE volume3 );
Q_PROPERTY( double panorama3 READ panorama3 WRITE set_panorama3 );
public:
	Mono2StereoElement2( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~Mono2StereoElement2();

	int inchannels() const { return 3; }
	int outchannels() const { return 2; }

	
	double volume() const { return _volume_value; }
	double panorama() const { return _balance_value; }
	//double volume2() const { return _volume_value2; }
	double panorama2() const { return _balance_value2; }
	//double volume3() const { return _volume_value3; }
	double panorama3() const { return _balance_value3; }
	
signals:
	void volume_changed( double );
	void panorama_changed( double );
	void volume_changed2( double );
	void panorama_changed2( double );
	void volume_changed3( double );
	void panorama_changed3( double );
public slots:
	void set_panorama( double n ) { balance( n ); }
	void balance( double );
	void volume( double );
	void set_panorama2( double n ) { balance2( n ); }
	void balance2( double );
	//void volume2( double );
	void set_panorama3( double n ) { balance3( n ); }
	void balance3( double );
	//void volume3( double );
private slots:
	void calculateVolumes();
	void calculateVolumes2();
	void calculateVolumes3();
private:
	QString _inchannel1, _inchannel2, _inchannel3, _outchannel1, _outchannel2;
	JackMix::GUI::Knob *_balance, *_balance2, *_balance3;
	JackMix::GUI::Slider *_volume_widget;
	double _balance_value, _volume_value, _balance_value2, _volume_value2, _balance_value3, _volume_value3;
};

void init_stereo_elements();

}; // MixerElements
}; // JackMix

#endif // STEREO_ELEMENTS_H

