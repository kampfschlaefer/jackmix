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

#include "connectionlister.h"
#include "connectionlister.moc"
#include "mixingmatrix.h"

#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlabel.h>

namespace JackMix {
namespace MixingMatrix {


ConnectionLister::ConnectionLister( Widget* w, QWidget* p, const char* n )
	: QWidget( p,n )
	, _widget( w )
	, _layout( new QGridLayout( this, 3,3, 5,5 ) )
	, _btn_connect( new QPushButton( "Connect", this ) )
	, _btn_close( new QPushButton( "Close", this ) )
	, _btn_disconnectMaster( new QPushButton( "Disconnect Master", this ) )
	, _btn_disconnectSlave( new QPushButton( "Disconnect Slave", this ) )
	, _box_signals( new QListView( this ) )
	, _box_slots( new QListView( this ) )
{
	qDebug( "ConnectionLister::ConnectionLister()" );
	QLabel *tmp = new QLabel( "Signals:", this );
	_layout->addWidget( tmp, 0,0 );
	tmp = new QLabel( "Slots:", this );
	_layout->addWidget( tmp, 0,2 );
	_layout->addWidget( _box_slots, 1,2 );
	_layout->addWidget( _box_signals, 1,0 );
	_layout->addWidget( _btn_disconnectMaster, 2,0 );
	_layout->addWidget( _btn_disconnectSlave, 2,2 );
	_layout->addWidget( _btn_connect, 3,1 );
	_layout->addWidget( _btn_close, 3,2 );

	_btn_connect->setDefault( true );
	_box_signals->addColumn( "Masters" );
	_box_signals->setRootIsDecorated( true );
	_box_slots->addColumn( "Slaves" );
	_box_slots->setRootIsDecorated( true );

	_btn_connect->setEnabled( false );
	_btn_disconnectMaster->setEnabled( false );
	_btn_disconnectSlave->setEnabled( false );
	connect( _btn_close, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( _btn_connect, SIGNAL( clicked() ), this, SLOT( connectControls() ) );
	connect( _btn_disconnectMaster, SIGNAL( clicked() ), this, SLOT( disconnectMaster() ) );
	connect( _btn_disconnectSlave, SIGNAL( clicked() ), this, SLOT( disconnectSlave() ) );
	connect( _box_slots, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
	connect( _box_signals, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );

	QValueList<Element*>::Iterator it;
	for ( it=_widget->_elements.begin(); it!=_widget->_elements.end(); ++it ) {
		//qDebug( "  %p : %s", ( *it ), ( *it )->getPropertyList().join( "," ).latin1() );
		QStringList tmp = ( *it )->getPropertyList();
		if ( !tmp.empty() ) {
			_box_signals->insertItem( new ElementConnectView( _box_signals, ( *it ) ) );
			_box_slots->insertItem( new ElementConnectView( _box_slots, ( *it ) ) );
		}
	}
}
ConnectionLister::~ConnectionLister() {
}

void ConnectionLister::connectControls() {
	qDebug( "ConnectionLister::connectControls()" );
	if ( _box_signals->selectedItem() && _box_signals->selectedItem()->rtti() == 5282 && _box_slots->selectedItem() && _box_slots->selectedItem()->rtti() == 5282 ) {
		ElementPropertyView* signal = static_cast<ElementPropertyView*>( _box_signals->selectedItem() );
		//qDebug( " Selected Signals: %s", signal->property().debug().latin1() );
		ElementPropertyView* slot = static_cast<ElementPropertyView*>( _box_slots->selectedItem() );
		//qDebug( " Selected Slots: %s", slot->property().debug().latin1() );
		_widget->connectMasterSlave( signal->property(), slot->property() );
	}
}
void ConnectionLister::disconnectMaster() {
	//qDebug( "ConnectionLister::disconnectMaster()" );
	if ( _box_signals->selectedItem() && _box_signals->selectedItem()->rtti() == 5282 ) {
		ElementPropertyView* master = static_cast<ElementPropertyView*>( _box_signals->selectedItem() );
		_widget->disconnectMaster( master->property() );
	}
}
void ConnectionLister::disconnectSlave() {
	//qDebug( "ConnectionLister::disconnectSlave()" );
	if ( _box_slots->selectedItem() && _box_slots->selectedItem()->rtti() == 5282 ) {
		ElementPropertyView* slave = static_cast<ElementPropertyView*>( _box_slots->selectedItem() );
		_widget->disconnectSlave( slave->property() );
	}
}
void ConnectionLister::selectionChanged() {
	//qDebug( "ConnectionLister::selectionChanged()" );
	_btn_disconnectMaster->setEnabled( _box_signals->selectedItem() );
	_btn_disconnectSlave->setEnabled( _box_slots->selectedItem() );
	_btn_connect->setEnabled( _box_slots->selectedItem() && _box_signals->selectedItem() );
}

ElementConnectView::ElementConnectView( QListView* p, Element* e ) : QListViewItem( p ), _element( e ) {
	//qDebug( "ElementConnectView::ElementConnectView( %p, %p )", p, e );
	setText( 0, QString( "%1" ).arg( int( e ) ) );
	setSelectable( false );
	QStringList tmp = e->getPropertyList();
	for ( uint i=0; i<tmp.count(); i++ ) {
		insertItem( new ElementPropertyView( this, tmp[ i ] ) );
	}
}
ElementConnectView::~ElementConnectView() {
}

ElementPropertyView::ElementPropertyView( ElementConnectView* p, QString s ) : QListViewItem( p ) {
	//qDebug( "ElementPropertyView::ElementPropertyView( %p, %s )", p, s.latin1() );
	setText( 0, s );
	_element = p->element();
	_property = s;
}
ElementPropertyView::~ElementPropertyView() {
}
ElementSlotSignalPair ElementPropertyView::property() const {
	return ElementSlotSignalPair( _element, _property );
}

}; // MixingMatrix
}; //JackMix

