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

#include "qlayout.h"
#include "qlistbox.h"
#include "qpushbutton.h"

namespace JackMix {
namespace MixingMatrix {


ConnectionLister::ConnectionLister( Widget* w, QWidget* p, const char* n )
	: QWidget( p,n )
	, _widget( w )
	, _layout( new QGridLayout( this, 2,3, 5,5 ) )
	, _btn_connect( new QPushButton( "Connect", this ) )
	, _btn_disconnect( new QPushButton( "Disconnect", this ) )
	, _box_signals( new QListBox( this ) )
	, _box_slots( new QListBox( this ) )
{
	qDebug( "ConnectionLister::ConnectionLister()" );
	_layout->addWidget( _box_slots, 0,0 );
	_layout->addWidget( _box_signals, 0,2 );
	_layout->addWidget( _btn_connect, 1,1 );
	_layout->addWidget( _btn_disconnect, 1,2 );

	QValueList<Element*>::Iterator it;
	for ( it=_widget->_elements.begin(); it!=_widget->_elements.end(); ++it ) {
		qDebug( "  %p : %s", ( *it ), ( *it )->getPropertyList().join( "," ).latin1() );
		QStringList tmp = ( *it )->getPropertyList();
		for ( uint i=0; i<tmp.count(); i++ ) {
			_box_slots->insertItem( QString( "%1 %2" ).arg( int( *it ) ).arg( tmp[ i ] ) );
			_box_signals->insertItem( QString( "%1 %2" ).arg( int( *it ) ).arg( tmp[ i ] ) );
		}
	}
}
ConnectionLister::~ConnectionLister() {
}


}; // MixingMatrix
}; //JackMix

