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

#include "osc_server.h"
#include "osc_server.moc"
#include <iostream>
#include <lo/lo_osc_types.h>

using namespace OSC;

Server::Server( QObject* p, const char* n ) : QObject( p,n ), _server( 0 ) {
	qDebug( "Server::Server( %p, %s )", p, n );
}
Server::Server( QString port, QObject* p, const char* n ) : QObject( p,n ), _server( 0 ) {
	qDebug( "Server::Server( %s, %p, %s )", port.latin1(), p, n );
	start( port );
}
Server::~Server() {
	std::cout << "Server::~Server()" << std::endl;
	stop();
}

void Server::start( QString port ) {
	if ( !_server ) {
		_server = lo_server_thread_new( port, OSC::error );
		lo_server_thread_add_method( _server, NULL, NULL, OSC::generic_handler, this );
		lo_server_thread_start( _server );
	}
}
void Server::stop() {
	if ( _server ) {
		lo_server_thread_stop( _server );
		_server = 0;
	}
}

void Server::data( const char* path, QVariant data ) {
	qDebug( "Server::data( %s, %s(%s) )", path, data.toString().latin1(), data.typeName() );
	emit gotData( path );
}

void OSC::error( int num, const char *msg, const char *path )
{
	printf( "liblo server error %d in path %s: %s\n", num, path, msg );
}

int OSC::generic_handler( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data )
{
	std::cout << "OSC::generic_handler()" << std::endl;
	int i;

	Server* _server = static_cast<Server*>( user_data );

	printf( "path: <%s>\n", path );
	for ( i=0; i<argc; i++ ) {
		printf( "arg %d '%c' ", i, types[ i ] );
		lo_arg_pp( lo_type( types[ i ] ), argv[ i ] );
		printf( "\n" );
	}
	if ( argc==1 ) {
		if ( lo_is_numerical_type( lo_type( types[ 0 ] ) ) )
			_server->data( path, double( lo_hires_val( lo_type( types[ 0 ] ), argv[ 0 ] ) ) );
//		else if ( lo_is_string_type( lo_type( types[ 0 ] ) ) )
//			_server->data( path, QString( strchr( lo_string_types, lo_type( argv[ 0 ] ) ) ) );
		else _server->data( path );
	} else
		_server->data( path );
	std::cout << data << std::endl;
	printf( "\n" );

	return 1;
}
