
#include "qlash.h"
#include "qlash.moc"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

namespace qLash {

qLashClient::qLashClient( QString clientname, int argc, char** argv, QObject* p ) : QObject( p ) {
	qDebug() << "qLashClient::qLashClient(" << clientname << "," << argc << "," << argv << "," << p << ")";
	if ( argc==0 && argv==0 ) {
		QStringList args = QCoreApplication::instance()->arguments();
		argc = args.size();
		QByteArray* arrays[ args.size() ];
		for ( int i=0; i<args.size(); ++i )
			arrays[ i ] = new QByteArray( args[ i ].toAscii() );
		argv = new char*[ args.size() ];
		for ( int i=0; i<args.size(); ++i )
			argv[ i ] = arrays[ i ]->data();
	}
	for ( int i=0; i<argc; ++i )
		qDebug() << i << "-" << argv[ i ];
	_client = lash_init( lash_extract_args( &argc, &argv ), clientname.toStdString().c_str(), LASH_Config_Data_Set | LASH_Config_File, LASH_PROTOCOL( 2,0 ) );

	if ( isConnected() ) {

		lash_event_t* event = lash_event_new_with_type( LASH_Client_Name );
		lash_event_set_string( event, clientname.toStdString().c_str() );
		lash_send_event( _client, event );

	}

	startTimer( 250 ); // Fire four times a second.
}
qLashClient::~qLashClient() {
}

bool qLashClient::isConnected() {
	return lash_enabled( _client );
}

void qLashClient::saveToDirFinalize( QString path ) {
	//qDebug() << "qLashClient::saveToDirFinalize()";
	QFile file( QString( "%1/_qlashvalues" ).arg( path ) );
	if ( file.open( QIODevice::WriteOnly ) ) {
		QDataStream out( &file );
		foreach( QString n, _values.keys() )
			out << n << _values[ n ];
		file.close();
	}
	lash_send_event( _client, lash_event_new_with_type( LASH_Save_File ) );
}
void qLashClient::saveToConfigFinalize() {
	//qDebug() << "qLashClient::saveToConfigFinalize()";
	lash_send_event( _client, lash_event_new_with_type( LASH_Save_Data_Set ) );
}
void qLashClient::restoreFromDirFinalize( QString path ) {
	//qDebug() << "qLashClient::restoreFromDirFinalize()";
	QFile file( QString( "%1/_qlashvalues" ).arg( path ) );
	if ( file.open( QIODevice::ReadOnly ) ) {
		QDataStream in( &file );
		while ( !in.atEnd() ) {
			QString n;
			QVariant v;
			in >> n >> v;
			_values[ n ] = v;
			emit valueChanged( n,v );
		}
		file.close();
	}
	lash_send_event( _client, lash_event_new_with_type( LASH_Restore_File ) );
}
void qLashClient::restoreFromConfigFinalize() {
	//qDebug() << "qLashClient::restoreFromConfigFinalize()";
	lash_send_event( _client, lash_event_new_with_type( LASH_Restore_Data_Set ) );
}

void qLashClient::setValue( QString n, QVariant v ) {
	_values[ n ] = v;
}
QVariant qLashClient::getValue( QString n ) const {
	return _values[ n ];
}

void qLashClient::setJackName( QString n ) {
	if ( isConnected() )
		lash_jack_client_name( _client, n.toStdString().c_str() );
}

void qLashClient::timerEvent( QTimerEvent* ) {
	while ( isConnected() && lash_get_pending_event_count( _client ) > 0 ) {
		lash_event_t* event = lash_get_event( _client );
		//qDebug() << "Have event of type" << lash_event_get_type( event );

		switch ( lash_event_get_type( event ) ) {
			case LASH_Client_Name:
				qDebug() << "Event: This clients name is:" << lash_event_get_string( event );
				break;
			case LASH_Jack_Client_Name:
				qDebug() << "Event: This clients JACK-name is:" << lash_event_get_string( event );
				break;
			case LASH_Alsa_Client_ID:
				qDebug() << "Event: This clients Alsa-ID is:" << lash_event_get_string( event );
				break;
			case LASH_Save_File:
				//qDebug() << "Event: Should save data into dir" << lash_event_get_string( event );
				emit saveToDir( lash_event_get_string( event ) );
				emit saveValues();
				saveToDirFinalize( lash_event_get_string( event ) );
				break;
			case LASH_Restore_File:
				//qDebug() << "Event: Should restore data from dir" << lash_event_get_string( event );
				emit restoreFromDir( lash_event_get_string( event ) );
				restoreFromDirFinalize( lash_event_get_string( event ) );
				emit restoreValues();
				break;
			case LASH_Save_Data_Set:
				//qDebug() << "Event: Should save data in configs";
				emit saveToConfig();
				//emit saveValues(); // The values are saved in their own file in the dir...
				saveToConfigFinalize();
				break;
			case LASH_Restore_Data_Set:
				//qDebug() << "Event: Should restore data from configs";
				emit restoreFromConfig();
				restoreFromConfigFinalize();
				break;
			case LASH_Quit:
				qDebug() << "Event: Should terminate now";
				emit quitApp();
				break;
			default:
				qDebug() << "Handling of event" << lash_event_get_type( event ) << "isn't implemented yet!";
		}

		lash_event_destroy( event );
	}
}

}; // qLash

