/*
    Copyright 2004 - 2007 Arnold Krille <arnold@arnoldarts.de>

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

#include "aux_elements.h"
#include "aux_elements.moc"

#include "knob.h"


#include <QtGui/QLayout>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QDebug>

using namespace JackMix;
using namespace JackMix::MixerElements;
using namespace JackMix::MixingMatrix;

/// The Factory for creating this things...
class AuxFactory : public JackMix::MixingMatrix::ElementFactory
{
public:
	AuxFactory() : ElementFactory() {
	qDebug() << "AuxFactory::AuxFactory()";
		globaldebug();
	}
	~AuxFactory() {}

	QStringList canCreate() const {
		return QStringList()<<"AuxElement";
	}
	QStringList canCreate( int in, int out ) const {
		if ( in==1 && out==1 ) return QStringList()<<"AuxElement";
		return QStringList();
	}

	Element* create( QString type , QStringList ins, QStringList outs, Widget* p, const char* n=0 ) {
		if ( type=="AuxElement" )
			return new AuxElement( ins, outs, p, n );
		else
			return 0;
	}
};

void MixerElements::init_aux_elements() {
	new AuxFactory();
}

AuxElement::AuxElement( QStringList inchannel, QStringList outchannel, MixingMatrix::Widget* p, const char* n )
	: Element( inchannel, outchannel, p, n )
	, dB2VolCalc( -42, 6 )
	, _inchannel( inchannel[ 0 ] )
	, _outchannel( outchannel[ 0 ] )
{
	menu()->addAction( "Select", this, SLOT( slot_simple_select() ) );
	menu()->addAction( "Replace", this, SLOT( slot_simple_replace() ) );
	QVBoxLayout* _layout = new QVBoxLayout( this );

	if ( _inchannel == _outchannel )
		_layout->addWidget(
			new QLabel( QString( "<qt><center>%1</center></qt>" ).arg( _inchannel ), this ),
			0 );

	JackMix::GUI::Knob* poti = new JackMix::GUI::Knob(
		amptodb( backend()->getVolume( _inchannel, _outchannel ) ),
		dbmin, dbmax, 2, 3, this );
	_layout->addWidget( poti, 100 );

	connect( poti, SIGNAL( valueChanged( double ) ), this, SLOT( emitvalue( double ) ) );
}
AuxElement::~AuxElement() {
}

void AuxElement::emitvalue( double n ) {
	backend()->setVolume( _inchannel, _outchannel, dbtoamp( n ) );
}

