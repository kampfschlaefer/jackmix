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

#include "abstractslider.h"
#include "abstractslider.moc"

#include <QtCore/QDebug>
#include <QtGui/QMouseEvent>
#include <QtGui/QDoubleSpinBox>

namespace JackMix {
namespace GUI {

AbstractSlider::AbstractSlider( double value, double min, double max, int precision, double pagestep, QWidget* p, QString valuestring )
	: QWidget( p )
	, dB2VolCalc( min, max )
	, _value( value ), _value_inupdate( false ), _pagestep( pagestep )
	, _precision( precision )
	, _valuestring( valuestring )
{
}
AbstractSlider::~AbstractSlider() {
}

void AbstractSlider::value( double v ) {
	if ( !_value_inupdate ) {
		v = qMin( v, dbmax );
		v = qMax( v, dbmin );
		_value = v;
		update();
		_value_inupdate = true;
		emit valueChanged( _value );
		_value_inupdate = false;
	}
}

void AbstractSlider::setNormalisedValue(double v, bool show_numeric) {
	v = qMax(v, 0.0);
	v = qMin(v, 1.0);
	// The call below will be handled by a virtual method in a derived class
	// The second argument enables or suppresses an auxilliary numeric display
	// of the slider's value.
	value(dbmin + v*(dbmax-dbmin), show_numeric);
}

void AbstractSlider::setMidiValue(int iv) {
	iv = qMax(iv, 0);
	iv = qMin(iv, 127);
	// Some cheap MIDI controllers "dither" (change randomly by 1 or more LSBs)
	// so we'll suppress any auxilliary display of slider value because it might
	// keep popping up annoyingly when the slider isn't being adjusted.
	setNormalisedValue(iv/127.0, false);
}


void AbstractSlider::hideInput() {
	if ( !_spinbox.isNull() ) {
		_spinbox->deleteLater();
	}
}
void AbstractSlider::showInput() {
	if ( _spinbox.isNull() ) {
		_spinbox = new QDoubleSpinBox( this );
		_spinbox->setMinimum( dbmin );
		_spinbox->setMaximum( dbmax );
		_spinbox->setSingleStep( _pagestep );
		_spinbox->setValue( _value );
		_spinbox->setFrame( false );
		connect( _spinbox, SIGNAL( editingFinished() ), this, SLOT( hideInput() ) );	//qDebug() << iv << "=>" << ((static_cast<double>(iv))/127.0);

		connect( _spinbox, SIGNAL( valueChanged( double ) ), this, SLOT( value( double ) ) );
		connect( this, SIGNAL( valueChanged( double ) ), _spinbox, SLOT( setValue( double ) ) );
		_spinbox->show();
		_spinbox->move( ( width()-_spinbox->width() )/2, ( height()-_spinbox->height() )/2 );
		_spinbox->setFocus();
	}
}

void AbstractSlider::contextMenuEvent( QContextMenuEvent* ev ) {
	//qDebug() << "AbstractSlider::contextMenuEvent(" << ev << ") is accepted?" << ev->isAccepted();
	if ( _spinbox.isNull() )
		showInput();
	else
		ev->ignore();
}	

void AbstractSlider::mousePressEvent( QMouseEvent* ev ) {
	//qDebug() << "AbstractSlider::mousePressEvent(" << ev << ") is accepted?" << ev->isAccepted();
	if ( ev->button() == Qt::LeftButton) {
		if (ev->modifiers() == Qt::ShiftModifier) {
			emit(select());
			ev->accept();
		} else if ( ev->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier) ) {
			emit(replace());
			ev->accept();
		} else {
			if ( _nullclick.contains( ev->pos() ) )
				value( 0 );
			else
				mouseEvent( ev );
		}
	}
}
void AbstractSlider::mouseMoveEvent( QMouseEvent* ev ) {
	mouseEvent( ev );
}
void AbstractSlider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta() > 0 )
		value( value() + _pagestep, true );
	else
		value( value() - _pagestep, true );
}

};
};
