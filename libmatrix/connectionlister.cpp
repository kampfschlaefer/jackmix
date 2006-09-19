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

#include <QtGui/QLayout>
//#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>

namespace JackMix {
namespace MixingMatrix {


ConnectionLister::ConnectionLister( Widget* w, QWidget* p, const char* n )
	: QWidget( p )
	, _widget( w )
	, _layout( new QGridLayout( this ) )
	, _btn_connect( new QPushButton( "Connect", this ) )
	, _btn_close( new QPushButton( "Close", this ) )
	, _btn_disconnectMaster( new QPushButton( "Disconnect Master", this ) )
	, _btn_disconnectSlave( new QPushButton( "Disconnect Slave", this ) )
	, _box_signals( new QListWidget( this ) )
	, _box_slots( new QListWidget( this ) )
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
	//_box_signals->addColumn( "Masters" );
	//_box_signals->setRootIsDecorated( true );
	//_box_slots->addColumn( "Slaves" );
	//_box_slots->setRootIsDecorated( true );

	_btn_connect->setEnabled( false );
	_btn_disconnectMaster->setEnabled( false );
	_btn_disconnectSlave->setEnabled( false );
	connect( _btn_close, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( _btn_connect, SIGNAL( clicked() ), this, SLOT( connectControls() ) );
	connect( _btn_disconnectMaster, SIGNAL( clicked() ), this, SLOT( disconnectMaster() ) );
	connect( _btn_disconnectSlave, SIGNAL( clicked() ), this, SLOT( disconnectSlave() ) );
	connect( _box_slots, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
	connect( _box_signals, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );

	QList<Element*>::Iterator it;
	for ( it=_widget->_elements.begin(); it!=_widget->_elements.end(); ++it ) {
		qDebug( "  %p : %s", ( *it ), ( *it )->getPropertyList().join( "," ).toStdString().c_str() );
		addElement( *it );
	}
}
ConnectionLister::~ConnectionLister() {
}

void ConnectionLister::addElement( Element* elem ) {
	qDebug( "ConnectionLister::addElement( %p [%s] )", elem, elem->getPropertyList().join( "," ).toStdString().c_str() );
	QStringList tmp = elem->getPropertyList();
	if ( !tmp.empty() ) {
		qDebug( "_box_signals->childCount() = %i", _box_signals->count() );
		_box_signals->addItem( new ElementConnectView( _box_signals, elem ) );
		qDebug( "_box_signals->childCount() = %i", _box_signals->count() );
		_box_slots->addItem( new ElementConnectView( _box_slots, elem ) );
	}
}
void ConnectionLister::removeElement( Element* elem ) {
//	qDebug( "ConnectionLister::removeElement( %p )", elem );
	//
	// TODO: Needs to be revised with new QList<QListWidgetItem>...
	//
/*	QListWidgetItem *tmp = _box_signals->item( 0 );
	while ( tmp != 0 ) {
		ElementConnectView* _tmp = static_cast<ElementConnectView*>( tmp );
		tmp = tmp->nextSibling();
		if ( elem == _tmp->element() )
			delete _tmp;
	}
	tmp = _box_slots->firstChild();
	while ( tmp != 0 ) {
		ElementConnectView* _tmp = static_cast<ElementConnectView*>( tmp );
		tmp = tmp->nextSibling();
		if ( elem == _tmp->element() )
			delete _tmp;
	}*/
}

void ConnectionLister::connectControls() {
	//qDebug( "ConnectionLister::connectControls()" );
	/*if ( _box_signals->currentItem() && _box_signals->currentItem()->type() == 5282 && _box_slots->currentItem() && _box_slots->currentItem()->type() == 5282 ) {
		ElementPropertyView* signal = static_cast<ElementPropertyView*>( _box_signals->currentItem() );
		//qDebug( " Selected Signals: %s", signal->property().debug().toStdString().c_str() );
		ElementPropertyView* slot = static_cast<ElementPropertyView*>( _box_slots->currentItem() );
		//qDebug( " Selected Slots: %s", slot->property().debug().toStdString().c_str() );
		_widget->connectMasterSlave( signal->property(), slot->property() );
	}*/
}
void ConnectionLister::disconnectMaster() {
	//qDebug( "ConnectionLister::disconnectMaster()" );
	/*if ( _box_signals->currentItem() && _box_signals->currentItem()->type() == 5282 ) {
		ElementPropertyView* master = static_cast<ElementPropertyView*>( _box_signals->currentItem() );
		_widget->disconnectMaster( master->property() );
	}*/
}
void ConnectionLister::disconnectSlave() {
	//qDebug( "ConnectionLister::disconnectSlave()" );
	/*if ( _box_slots->currentItem() && _box_slots->currentItem()->type() == 5282 ) {
		ElementPropertyView* slave = static_cast<ElementPropertyView*>( _box_slots->currentItem() );
		_widget->disconnectSlave( slave->property() );
	}*/
}
void ConnectionLister::selectionChanged() {
	//qDebug( "ConnectionLister::selectionChanged()" );
	_btn_disconnectMaster->setEnabled( _box_signals->currentItem() );
	_btn_disconnectSlave->setEnabled( _box_slots->currentItem() );
	_btn_connect->setEnabled( _box_slots->currentItem() && _box_signals->currentItem() );
}

ElementConnectView::ElementConnectView( QListWidget* p, Element* e ) : QListWidgetItem( p, 5282 ), _element( e ) {
	//qDebug( "ElementConnectView::ElementConnectView( %p, %p )", p, _element );
	setText( QString( "%1" ).arg( qint64( e ) ) );
	//setSelectable( false );
	// TODO
/*	QStringList tmp = e->getPropertyList();
	for ( int i=0; i<tmp.count(); i++ ) {
		insertItem( new ElementPropertyView( this, tmp[ i ] ) );
	}*/
}
ElementConnectView::~ElementConnectView() {
}

/*ElementPropertyView::ElementPropertyView( ElementConnectView* p, QString s ) : QListWidgetItem( p, 5282 ) {
	//qDebug( "ElementPropertyView::ElementPropertyView( %p, %s )", p, s.toStdString().c_str() );
	setText( 0, s );
	_element = p->element();
	_property = s;
}
ElementPropertyView::~ElementPropertyView() {
}
ElementSlotSignalPair ElementPropertyView::property() const {
	return ElementSlotSignalPair( _element, _property );
}*/

}; // MixingMatrix
}; //JackMix

