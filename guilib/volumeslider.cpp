/*
    Copyright ( C ) 2004 Arnold Krille <arnold@arnoldarts.de>

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

#include "volumeslider.h"
#include "volumeslider.moc"

#include <iostream>
#include <qlayout.h>
#include <qlabel.h>
#include "qfloatslider.h"
#include "qfloatpoti.h"

using namespace JackMix;

VolumeSlider::VolumeSlider( QString name, float value, Direction dir, QWidget* p, bool showlabel, bool showvalue, const char* n )
 : QFrame( p,n )
 , dB2VolCalc( -36, 12 )
 , _name( name )
 , _showlabel( showlabel )
 , _showvalue( showvalue )
{
	QBoxLayout *_layout = new QBoxLayout( this, QBoxLayout::TopToBottom );
	QFloatSlider *tmp = new QFloatSlider(  value, dbmin, dbmax, 0.1, 1000, dir, this );
	connect( tmp, SIGNAL( valueChanged( float ) ), this, SLOT( iValueChanged( float ) ) );
	if ( _showlabel )
		_layout->addWidget( new QLabel( _name, this ),-10,Qt::AlignCenter );
	_layout->addWidget( tmp,100 );
}
VolumeSlider::~VolumeSlider() {
}

void VolumeSlider::iValueChanged( float n ) {
	//std::cerr << "VolumeSlider::iValueChanged( " << n << ")" << std::endl;
	//std::cerr << "That is an ampfactor of: " << dbtoamp( n ) << std::endl;
	emit valueChanged( _name, dbtoamp( n ) );
}

VolumeKnob::VolumeKnob( QString name, float value, QWidget* p, bool _showlabel, const char* n ) : QFrame( p,n ), dB2VolCalc( -36,12 ), _name( name )
{
	QBoxLayout *_layout = new QBoxLayout( this, QBoxLayout::TopToBottom );
	QFloatPoti *tmp = new QFloatPoti( value, dbmin, dbmax, 10, QColor( 255,0,0 ), this );
	connect( tmp, SIGNAL( valueChanged( float ) ), this, SLOT( iValueChanged( float ) ) );
	if ( _showlabel )
		_layout->addWidget( new QLabel( _name, this ),-10,Qt::AlignCenter );
	_layout->addWidget( tmp,100 );
}
VolumeKnob::~VolumeKnob() {
}

void VolumeKnob::iValueChanged( float n ) {
	emit valueChanged( _name, dbtoamp( n ) );
}
