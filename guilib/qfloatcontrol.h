#ifndef QFLOATCONTROL_H
#define QFLOATCONTROL_H

#include <qframe.h>

namespace JackMix {
namespace GUI {

class QFloatControl : public QFrame
{
Q_OBJECT
Q_PROPERTY( float min READ min WRITE min )
Q_PROPERTY( float max READ max WRITE max )
public:
	QFloatControl( QWidget*, const char* =0 );
	~QFloatControl();

	void paintEvent( QPaintEvent* );

	void min( float ) {}
	void max( float ) {}
	void value( float ) {}
	float min() const { return _min; }
	float max() const { return _max; }
	float value() const { return _value; }
private:
	float _min, _max, _value;
};

}; // GUI
}; // JackMix

#endif // QFLOATCONTROL_H

