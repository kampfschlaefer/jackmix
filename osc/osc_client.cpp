/*
    Copyright ( C ) 2005 Arnold Krille <arnold@arnoldarts.de>

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

#include "osc_client.h"
#include "osc_client.moc"

using namespace OSC;

Client::Client( QObject* p, const char* n ) : QObject( p ), _connection( 0 ) {
	qDebug( "Client::Client( QObject %p, const char* %s )", p, n );
	qDebug( "Client::Client() _connection = %p", _connection );
}
Client::Client( QString url, QObject* p, const char* n ) : QObject( p ), _connection( 0 ) {
	qDebug( "Client::Client( url %s, QObject %p, const char* %s )", url.toStdString().c_str(), p, n );
	connect( lo_address_new_from_url( url.toStdString().c_str() ) );
	qDebug( "Client::Client() _connection = %p", _connection );
}
Client::Client( QString host, QString port, QObject* p, const char* n ) : QObject( p ), _connection( 0 ) {
	qDebug( "Client::Client( host %s, port %s, QObject %p, const char* %s )", host.toStdString().c_str(), port.toStdString().c_str(), p, n );
	connect( host, port );
	qDebug( "Client::Client() _connection = %p", _connection );
}
Client::~Client() {
}

void Client::connect( QString host, QString port ) {
	qDebug( "Client::connect( %s, %s )", host.toStdString().c_str(), port.toStdString().c_str() );
	connect( lo_address_new( host.toStdString().c_str(), port.toStdString().c_str() ) );
}
void Client::connect( lo_address address ) {
	_connection = address;
	qDebug( "error: %s", lo_address_errstr( _connection ) );
//	qDebug( "_connection: %p", _connection );
	if ( _connection ) emit connected();
}

void Client::sendData( QString path, QVariant data ) {
	qDebug( "Client::sendData( %s, %s(%s) )", path.toStdString().c_str(), data.toString().toStdString().c_str(), data.typeName() );
	if ( _connection ) {
		if ( !data.isNull() ) {
			if ( data.type() == QVariant::String )
				/*qDebug( "Return %i",*/ lo_send( _connection, path.toStdString().c_str(), "s", data.toString().toStdString().c_str() )/* )*/;
			else
				if ( data.type()==QVariant::Double )
				/*qDebug( "Return %i",*/ lo_send( _connection, path.toStdString().c_str(), "d", data.toDouble() )/* )*/;
			else
				if ( data.type()==QVariant::Int || data.type()==QVariant::UInt )
				/*qDebug( "Return %i",*/ lo_send( _connection, path.toStdString().c_str(), "i", data.toInt() )/* )*/;
		} else {
			/*qDebug( "Return %i",*/ lo_send( _connection, path.toStdString().c_str(), NULL )/* )*/;
		}
		qDebug( "Error of the last command: %i:%s", lo_address_errno( _connection ), lo_address_errstr( _connection ) );
		if ( lo_address_errno( _connection ) == 111 ) {
			_connection = 0;
			emit disconnected();
			emit disconnected( this );
			qWarning( "Got disconnected!" );
		}
	} else qWarning( "Not Connected!" );
}

ClientPath::ClientPath( Client* client, QString path, QVariant::Type type )
	: QObject( client )
	, _client( client )
	, _path( path )
	, _type( type )
{
	qDebug( "ClientPath: Size of _paths: %i", _client->_paths.size() );
	_client->_paths.insert( _path, this );
	qDebug( "ClientPath: Size of _paths: %i", _client->_paths.size() );
}
ClientPath::~ClientPath() {
	qDebug( "~ClientPath: Size of _paths: %i", _client->_paths.size() );
	_client->_paths.remove( _path );
	qDebug( "~ClientPath: Size of _paths: %i", _client->_paths.size() );
}

void ClientPath::data() { _client->sendData( _path ); }
void ClientPath::data( QString d ) { _client->sendData( _path, d ); }
void ClientPath::data( int d ) { _client->sendData( _path, d ); }
