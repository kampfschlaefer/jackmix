#ifndef AUX_ELEMENTS_H
#define AUX_ELEMENTS_H

#include <mixingmatrix.h>
#include <dbvolcalc.h>

namespace JackMix {
namespace MixerElements {

class AuxElement;
class Mono2StereoElement;
class Stereo2StereoElement;

/**
 * Simpliest form of a control connecting one input with one output.
 */
class AuxElement : public JackMix::MixingMatrix::Element, public dB2VolCalc
{
Q_OBJECT
public:
	AuxElement( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~AuxElement();

	int inchannels() const { return 1; }
	int outchannels() const { return 1; }

public slots:
	void slot_toggle();
	void slot_replace();
	void emitvalue( float );

private:
	QString _inchannel, _outchannel;
};

/**
 * A MonotoStereo control.
 */
class Mono2StereoElement : public JackMix::MixingMatrix::Element, public dB2VolCalc
{
Q_OBJECT
public:
	Mono2StereoElement( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~Mono2StereoElement();

	int inchannels() const { return 1; }
	int outchannels() const { return 2; }

private slots:
	void slot_toggle();
	void balance( float );
	void volume( float );
	void calculateVolumes();
private:
	QString _inchannel, _outchannel1, _outchannel2;
	float _balance_value, _volume_value;
};

/**
 * A StereotoStereo control.
 */
class Stereo2StereoElement : public JackMix::MixingMatrix::Element, public dB2VolCalc
{
Q_OBJECT
public:
	Stereo2StereoElement( QStringList, QStringList, MixingMatrix::Widget*, const char* =0 );
	~Stereo2StereoElement();

	int inchannels() const { return 2; }
	int outchannels() const { return 2; }

private slots:
	void slot_toggle();
	void slot_replace() { emit replace( this ); }
	void balance( float );
	void volume( float );
	void calculateVolumes();
private:
	QString _inchannel1, _inchannel2, _outchannel1, _outchannel2;
	float _balance_value, _volume_value;
};

}; // MixerElements
}; // JackMix

#endif // AUX_ELEMENTS_H

