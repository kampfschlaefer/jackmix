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

#include "vg_aux.h"
#include "vg_aux.moc"
#include "vg_aux_private.h"
#include "vg_aux_private.moc"

#include "jack_backend.h"
#include "volumeslider.h"
#include "qtickmarks.h"

#include <iostream>
#include <qlabel.h>
#include <qlayout.h>

using namespace JackMix;

VGAux::VGAux( QString _name, int ch, QObject* p, const char* n )
 : VolumeGroup( _name, ch, p,n )
{
std::cerr << "VGAux::VGAux( " << name() << ", " << channels() << ", " << p << ", n )" << std::endl;
	for ( int i=0; i<channels(); i++ ) {
		BACKEND->addOutput( name() + "_" + QString::number( i ) );
	}
	VolumeGroupFactory::the()->registerGroup( this );
}
VGAux::~VGAux() {
	std::cerr << "VGAux::~VGAux()" << std::endl;
	for ( int i=0; i<channels(); i++ ) {
		BACKEND->removeOutput( name() + "_" + QString::number( i ) );
	}
}

VolumeGroupMasterWidget* VGAux::masterWidget( QWidget* parent ) {
	if ( !_masterwidget )
		_masterwidget = new VGAuxMasterWidget( this, parent );
	return _masterwidget;
}
VolumeGroupChannelWidget* VGAux::channelWidget( QString name, QWidget* parent ) {
std::cerr << "VGAux::channelWidget( " << name << ", " << parent << " )" << std::endl;
	return new VGAuxChannelWidget( name, this, parent );
}

VGAuxMasterWidget::VGAuxMasterWidget( VGAux* g, QWidget* p, const char* n )
 : VolumeGroupMasterWidget( g, p,n )
{
	setMargin( 1 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	QGridLayout* _layout = new QGridLayout( this, 1, 2 );
	layout()->addItem( _layout );
	_layout->setMargin( 1 );
	//_layout->setAutoAdd( true );
	VolumeKnob* tmp;
	for ( int i=0; i<group()->channels(); i++ ) {
		tmp = new VolumeKnob( group()->name() + "_" + QString::number( i ), 1, this );
		connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( newValue( QString,float ) ) );
		_layout->addWidget( tmp, i/2, i%2 );
		//_layout->addWidget( new QTickmarks( -36, 12, LeftToRight, posLeft, this, 7 ) );
	}
}
VGAuxMasterWidget::~VGAuxMasterWidget() {
}
void VGAuxMasterWidget::newValue( QString ch, float n ) {
	BACKEND->setOutVolume( ch, n );
}


VGAuxChannelWidget::VGAuxChannelWidget( QString in, VGAux* g, QWidget* p, const char* n )
 : VolumeGroupChannelWidget( in, g, p,n )
{
	setMargin( 1 );
	setLineWidth( 1 );
	setFrameStyle( QFrame::Panel|QFrame::Raised );
	QGridLayout* _layout = new QGridLayout( this, 1,2 );
	_layout->setMargin( 1 );
	_layout->setAutoAdd( true );
	VolumeKnob* tmp;
	for ( int i=0; i<group()->channels(); i++ ) {
		tmp = new VolumeKnob( group()->name() + "_" + QString::number( i ), 1, this );
		connect( tmp, SIGNAL( valueChanged( QString,float ) ), this, SLOT( valueChanged( QString,float ) ) );
//		_layout->add( tmp );
//		_layout->addWidget( new QTickmarks( -36, 12, LeftToRight, posLeft, this, 7 ) );
	}
}
VGAuxChannelWidget::~VGAuxChannelWidget() {
}
void VGAuxChannelWidget::valueChanged( QString channel, float value ) {
	//std::cerr << "VolumeGroupChannelWidget::valueChanged( " << channel << ", " << value << " )" << std::endl;
	BACKEND->setVolume( inchannel(), channel, value );
}

