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
		connect( _spinbox, SIGNAL( editingFinished() ), this, SLOT( hideInput() ) );
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
	if ( ev->button() == Qt::LeftButton ) {
		if ( _nullclick.contains( ev->pos() ) )
			value( 0 );
		else
			mouseEvent( ev );
	}
}
void AbstractSlider::mouseMoveEvent( QMouseEvent* ev ) {
	mouseEvent( ev );
}
void AbstractSlider::wheelEvent( QWheelEvent* ev ) {
	if ( ev->delta() > 0 )
		value( value() + _pagestep );
	else
		value( value() - _pagestep );
}

};
};
