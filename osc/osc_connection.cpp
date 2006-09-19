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

#include "osc_connection.h"
#include "osc_connection.moc"

#include "osc_server.h"
#include "osc_client.h"

using namespace OSC;

ConnectionServer::ConnectionServer( QString port, QObject* p, const char* n )
	: QObject( p )
	, _server( new Server( port, this ) )
{
	qDebug( "ConnectionServer::ConnectionServer( %s, %p, %s )", port.toStdString().c_str(), p, n );
	// Müsste StringList sein, aber String für URL geht auch...
	_newClient = new ServerPath( _server, "/newclient", QVariant::String );
	connect( _newClient, SIGNAL( data( QString ) ), this, SLOT( newClient( QString ) ) );
	connect( _server, SIGNAL( gotData( QString, QVariant ) ), this, SLOT( forwardData( QString, QVariant ) ) );
}
ConnectionServer::~ConnectionServer() {
	qDebug( "ConnectionServer::~ConnectionServer()" );
	delete _server;
}

ServerPath* ConnectionServer::newServerPath( QString path, QVariant::Type type, bool forward ) {
	if ( forward )
		_forwardpaths.append( path );
	return new ServerPath( _server, path, type );
}

void ConnectionServer::newClient( QString url ) {
	qDebug( "ConnectionServer::newClient( %s )", url.toStdString().c_str() );
	_clients.append( new Client( url, this ) );
}
void ConnectionServer::clientDisconnected( Client* client ) {
	_clients.removeAll( client );
}

void ConnectionServer::sendData( QString path, QVariant data ) {
	qDebug( "ConnectionServer::sendData( %s, %s )", path.toStdString().c_str(), data.toString().toStdString().c_str() );
	ClientList::iterator it;
	for ( it = _clients.begin(); it != _clients.end(); ++it ) {
		( *it )->sendData( path, data );
	}
}

void ConnectionServer::forwardData( QString path, QVariant data ) {
	qDebug( "ConnectionServer::forwardData( %s, %s )", path.toStdString().c_str(), data.toString().toStdString().c_str() );
	if ( _forwardpaths.contains( path ) )
		sendData( path, data );
}

ConnectionClient::ConnectionClient( QString host, QString port, QObject* p, const char* n )
	: QObject( p )
	, _server( new OSC::Server( QString::number( rand() ), this ) )
	, _client( new OSC::Client( host, port, this ) )
{
	qDebug( "ConnectionClient::ConnectionClient( %s, %s, %p, %s )", host.toStdString().c_str(), port.toStdString().c_str(), p, n );
	_client->sendData( "/newclient", _server->url().toStdString().c_str() );
}
ConnectionClient::~ConnectionClient() {
	qDebug( "ConnectionClient::~ConnectionClient()" );
	delete _server;
	delete _client;
}

ClientPath* ConnectionClient::newClientPath( QString path, QVariant::Type type ) {
	return new ClientPath( _client, path, type );
}
ServerPath* ConnectionClient::newServerPath( QString path, QVariant::Type type ) {
	return new ServerPath( _server, path, type );
}


