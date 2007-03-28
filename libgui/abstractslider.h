/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library. If not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#ifndef ABSTRACTSLIDER_H
#define ABSTRACTSLIDER_H

#include <QtGui/QWidget>
#include <QtCore/QPointer>
#include "dbvolcalc.h"

class QDoubleSpinBox;

namespace JackMix {

namespace GUI {

class AbstractSlider : public QWidget, public dB2VolCalc
{
Q_OBJECT
public:
	AbstractSlider( double value, double min, double max, int precision, double pagestep, QWidget*, QString valuestring );
	~AbstractSlider();

public slots:
	virtual void value( double );
	virtual double value() const { return _value; };

signals:
	void valueChanged( double );

private slots:
	void hideInput();
	void showInput();

protected:
	void mousePressEvent( QMouseEvent* );
	void mouseMoveEvent( QMouseEvent* );
	void wheelEvent( QWheelEvent* );

	void contextMenuEvent( QContextMenuEvent* );

	virtual void mouseEvent( QMouseEvent* ) =0;


	double _value;
	bool _value_inupdate;
	double _pagestep;
	int _precision;
	QString _valuestring;

	QPointer<QDoubleSpinBox> _spinbox;

	QRegion _nullclick;
};

};

};

#endif // ABSTRACTSLIDER_H

