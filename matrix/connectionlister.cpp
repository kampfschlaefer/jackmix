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
//#include "qjackctl_connect.h"

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

	connect( _btn_close, SIGNAL( clicked() ), this, SLOT( close() ) );
	connect( _btn_connect, SIGNAL( clicked() ), this, SLOT( connectControls() ) );
	//connect( _btn_disconnect, SIGNAL( clicked() ), this, SLOT( disconnectControls() ) );

	QValueList<Element*>::Iterator it;
	for ( it=_widget->_elements.begin(); it!=_widget->_elements.end(); ++it ) {
		//qDebug( "  %p : %s", ( *it ), ( *it )->getPropertyList().join( "," ).latin1() );
		QStringList tmp = ( *it )->getPropertyList();
		if ( !tmp.empty() ) {
			_box_signals->insertItem( new ElementConnectView( _box_signals, ( *it ) ) );
			_box_slots->insertItem( new ElementConnectView( _box_slots, ( *it ) ) );
		}
	}

	//qjackctlConnectView *view = new qjackctlConnectView( this );
	//_layout->addMultiCellWidget( view, 0,0, 0,2 );
}
ConnectionLister::~ConnectionLister() {
}

void ConnectionLister::connectControls() {
	qDebug( "ConnectionLister::connectControls()" );
	if ( _box_signals->selectedItem() && _box_signals->selectedItem()->rtti() == 5282 ) {
		ElementPropertyView* tmp = static_cast<ElementPropertyView*>( _box_signals->selectedItem() );
		qDebug( " Selected Signals: %s", tmp->property().debug().latin1() );
	}
/*	if ( _box_signals->currentText() != "" && _box_slots->currentText() != "" ) {
		qDebug( "Selected signals: %s\nSelected slots: %s", _box_signals->currentText().latin1(), _box_slots->currentText().latin1() );
		QString master = _box_signals->currentText();
		QString master_element = master.section( " ", 0, 0 );
		QString master_signal = master.section( " ", 1, 1 );
		qDebug( "Master: (%p,%s)", ( Element* )master_element.toInt(), master_signal.latin1() );
		QString slave = _box_slots->currentText();
		QString slave_element = slave.section( " ", 0, 0 );
		QString slave_signal = slave.section( " ", 1, 1 );
		qDebug( "Slave: (%p,%s)", ( Element* )slave_element.toInt(), slave_signal.latin1() );
		_widget->connectMasterSlave( ( Element* )master_element.toInt(), master_signal, ( Element* )slave_element.toInt(), slave_signal );
	}*/
}
void ConnectionLister::disconnectControls() {
	qDebug( "ConnectionLister::disconnectControls()" );
	//qDebug( "Selected signals: %s\nSelected slots: %s", _box_signals->currentText().latin1(), _box_slots->currentText().latin1() );
/*	if ( _box_signals->currentText() != "" && _box_slots->currentText() != "" ) {
		QString master = _box_signals->currentText();
		QString master_element = master.section( " ", 0, 0 );
		QString master_signal = master.section( " ", 1, 1 );
		//qDebug( "Master: (%p,%s)", master_element.toInt(), master_signal.latin1() );
		QString slave = _box_slots->currentText();
		QString slave_element = slave.section( " ", 0, 0 );
		QString slave_signal = slave.section( " ", 1, 1 );
		//qDebug( "Slave: (%p,%s)", slave_element.toInt(), slave_signal.latin1() );
		//_widget->disconnectMasterSlave( ( Element* )master_element.toInt(), master_signal, ( Element* )slave_element.toInt(), slave_signal );
	}*/
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

