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

#include "oscserver.h"
#include "oscserver.moc"
#include <iostream>

using namespace JackMix::OSC;

Server::Server() : QObject( 0 ) {
	std::cout << "Server::Server()" << std::endl;
	_server = lo_server_thread_new( "5282", JackMix::OSC::error );
	lo_server_thread_add_method( _server, NULL, NULL, JackMix::OSC::generic_handler, this );
	lo_server_thread_start( _server );
}
Server::~Server() {
	std::cout << "Server::~Server()" << std::endl;
	lo_server_thread_stop( _server );
}

void JackMix::OSC::error( int num, const char *msg, const char *path )
{
	printf( "liblo server error %d in path %s: %s\n", num, path, msg );
}

int JackMix::OSC::generic_handler( const char *path, const char *types, lo_arg **argv, int argc, void *data, void *user_data )
{
	std::cout << "JackMix::OSC::generic_handler()" << std::endl;
	int i;

	Server* _server = static_cast<Server*>( user_data );

	printf( "path: <%s>\n", path );
	_server->data( path );
	for ( i=0; i<argc; i++ ) {
		printf( "arg %d '%c' ", i, types[ i ] );
		//lo_arg_pp( types[ i ], argv[ i ] );
		printf( "\n" );
	}
	printf( "\n" );

	return 1;
}
