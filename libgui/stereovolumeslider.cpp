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

#include "stereovolumeslider.h"
#include "stereovolumeslider.moc"

#include "qfloatslider.h"
#include "qtickmarks.h"
#include <QtGui/QLayout>

using namespace JackMix;

StereoVolumeSlider::StereoVolumeSlider( QString ch1, QString ch2, float _dbmin, float _dbmax, QWidget* p, const char* n )
 : QFrame( p )
 , dB2VolCalc( _dbmin, _dbmax )
 , _balance( 0 )
 , _volume( 1 )
 , _channel1( ch1 )
 , _channel2( ch2 )
{
	QVBoxLayout* layout = new QVBoxLayout( this );
	QFloatSlider* tmp = new QFloatSlider( 0,-1,1,0.1,200, LeftToRight,this );
	connect( tmp, SIGNAL( valueChanged( float ) ), this, SLOT( balanceChanged( float ) ) );
	layout->addWidget( tmp );
	QHBoxLayout* layout2 = new QHBoxLayout( this );
	layout->addLayout( layout2 );
	layout2->addWidget( new QTickmarks( dbmin, dbmax, BottomToTop, posRight, this, 7 ) );
	tmp = new QFloatSlider( 0, dbmin, dbmax, 0.1, 100, BottomToTop, this );
	connect( tmp, SIGNAL( valueChanged( float ) ), this, SLOT( volumeChanged( float ) ) );
	layout2->addWidget( tmp );
	layout2->addWidget( new QTickmarks( dbmin, dbmax, BottomToTop, posLeft, this, 7 ) );
}
StereoVolumeSlider::~StereoVolumeSlider() {
}

void StereoVolumeSlider::balanceChanged( float n ) {
	_balance = n;
	updateVolumes();
}
void StereoVolumeSlider::volumeChanged( float n ) {
	_volume = dbtoamp( n );
	updateVolumes();
}

void StereoVolumeSlider::updateVolumes() {
	float vol1, vol2;
	if ( _balance > 0 ) {
		vol1 = _volume * ( 1 - _balance );
		vol2 = _volume;
	} else { /// _balance < 0
		vol1 = _volume;
		vol2 = _volume * ( 1 + _balance );
	}
	emit valueChanged( _channel1, vol1 );
	emit valueChanged( _channel2, vol2 );
}

