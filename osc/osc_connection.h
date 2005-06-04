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
#ifndef OSC_CONNECTION_H
#define OSC_CONNECTION_H

#include <qobject.h>
#include <qvaluelist.h>
#include <qvariant.h>

namespace OSC {

	class Server;
	class ServerPath;
	class Client;
	class ClientPath;

	class ConnectionServer : public QObject {
	Q_OBJECT
	public:
		ConnectionServer( QString port, QObject*, const char* =0 );
		~ConnectionServer();
		ServerPath* newServerPath( QString path, QVariant::Type type );
	public slots:
		void sendData( QString path, QVariant data=QVariant() );
	private slots:
		void newClient( QString );
		void clientDisconnected( Client* );
	private:
		Server* _server;
		typedef QValueList <Client*> ClientList;
		ClientList _clients;
		ServerPath *_newClient;
	};

	class ConnectionClient : public QObject {
	Q_OBJECT
	public:
		ConnectionClient( QString host, QString port, QObject*, const char* =0 );
		~ConnectionClient();
		ClientPath* newClientPath( QString path, QVariant::Type type );
		ServerPath* newServerPath( QString path, QVariant::Type type );
		Client* client() { return _client; }
	private:
		Server* _server;
		Client* _client;
	};

};

#endif // OSC_CONNECTION_H

