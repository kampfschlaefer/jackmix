/*
    Copyright 2007 Arnold Krille <arnold@arnoldarts.de>

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
    along with this library. If not, write to the Free Software Foundation,
    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

*/

#ifndef KNOB_H
#define KNOB_H

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtGui/QColor>
#include <QtGui/QPaintEvent>
#include <QtGui/QPalette>

#include "abstractslider.h"

class QTimer;

namespace JackMix {
namespace GUI {

class Knob : public AbstractSlider
{
Q_OBJECT
Q_PROPERTY(QColor _indicator READ getIndicatorColor WRITE setIndicatorColor)
public:
	Knob( double value, double min, double max, int precision,
              double pagestep, QWidget*p, QString valuestring = "%1 dB" ) :
                Knob( value, min, max, precision, pagestep, p, valuestring, QColor(0,0,0) ) {
                _indicator = palette().color(QPalette::Highlight);
        };
        Knob( double value, double min, double max, int precision,
              double pagestep, QWidget*, QString valuestring, QColor indicator );
	~Knob();

	virtual void value( double n, bool show_numeric = true );
        QColor getIndicatorColor() const { return _indicator; };
        void setIndicatorColor(const QColor& c);
 
protected:
	void paintEvent( QPaintEvent* );

private slots:
	void timeOut();

private:
	void mouseEvent( QMouseEvent* );

	QTimer *_timer;
	bool _show_value;
        QColor _indicator;
};

}; // GUI
}; // JackMix
#endif // KNOB_H

