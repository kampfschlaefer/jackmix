#ifndef SLIDER_H
#define SLIDER_H

#include <qwidget.h>
#include "float_base.h"
#include "dbvolcalc.h"

namespace JackMix {
namespace GUI {

class Slider : public QWidget, public dB2VolCalc//, public FloatElement
{
Q_OBJECT
public:
	Slider( float value, float min, float max, int precision, float pagestep, QWidget*, QString = "%1 dB", const char* name=0 );
	~Slider();

	QSizePolicy sizePolicy() const { return QSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding ); }

	void paintEvent( QPaintEvent* );
	void mousePressEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void wheelEvent( QWheelEvent* );

	void value( float );
	float value() const { return _value; }
signals:
	void valueChanged( float );
private:
	float _value, _pagestep;
	int _precision;
	QString _valuestring;
};

}; // GUI
}; //JackMix

#endif // SLIDER_H

