
#ifndef FLOATSLIDER_H
#define FLOATSLIDER_H

#include <qwidget.h>

class QSlider;

class QFloatSlider : public QWidget {
Q_OBJECT
public:
	QFloatSlider( QWidget* =0, const char* =0 );
	QFloatSlider( Orientation o, QWidget* p=0, const char* n=0 );
	QFloatSlider( float value, float min, float max, float pagestep, int precision, Orientation, QWidget* =0, const char* =0 );
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
	void init( float value, float min, float max, float pagestep, int precision, Orientation );
	QSlider* _slider;
	float _value, _min, _max, _pagestep;
	int _precision;
};

#endif

