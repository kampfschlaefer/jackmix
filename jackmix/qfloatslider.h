
#ifndef FLOATSLIDER_H
#define FLOATSLIDER_H

#include <qframe.h>
#include "common.h"

class QSlider;

class QFloatSlider : public QFrame {
Q_OBJECT
public:
	QFloatSlider( QWidget* =0, const char* =0 );
	QFloatSlider( JackMix::Direction, QWidget* p=0, const char* n=0 );
	QFloatSlider( float value, float min, float max, float pagestep, int precision, JackMix::Direction, QWidget* =0, const char* =0 );
	~QFloatSlider();
public slots:
	void setPrecision( int );
	void setPageStep( float );
	void setMinimum( float );
	void setMaximum( float );
	void setValue( float );
signals:
	void valueChanged( float );
private slots:
	void setValue( int );
private:
	void init( float value, float min, float max, float pagestep, int precision, JackMix::Direction );
	void setMinMax();
	QSlider* _slider;
	float _value, _min, _max, _pagestep;
	int _precision;
	JackMix::Direction _dir;
};

#endif

