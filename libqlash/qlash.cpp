
#include "qlash.h"

#include <QtCore/QDebug>

namespace qLash {

qLashClient::qLashClient( QString clientname, int argc, char** argv, QObject* p ) : QObject( p ) {
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

void qLashClient::saveToDirFinished() {
	lash_send_event( _client, lash_event_new_with_type( LASH_Save_File ) );
}
void qLashClient::saveToConfigFinalize() {
	qDebug() << "qLashClient::saveToConfigFinalize()";
	foreach ( QString n, _values.keys() ) {
		qDebug() << " saving \"" << n << "," << _values[ n ] << "\"";
		QByteArray tmp;
		{
			QDataStream out( &tmp, QIODevice::WriteOnly );
			out << _values[ n ];
		}
		qDebug() << " string will be [" << tmp.data() << "]" << tmp.length();
		lash_config_t* config = lash_config_new_with_key( n.toStdString().c_str() );
		lash_config_set_value( config, tmp.data(), sizeof( tmp.data() ) );
		lash_send_config( _client, config );
	}
	lash_send_event( _client, lash_event_new_with_type( LASH_Save_Data_Set ) );
}
void qLashClient::restoreFromDirFinished() {
	lash_send_event( _client, lash_event_new_with_type( LASH_Restore_File ) );
}
void qLashClient::restoreFromConfigFinalize() {
	qDebug() << "qLashClient::restoreFromConfigFinalize()";
	lash_config_t* config =0;
	while ( config = lash_get_config( _client ) ) {
		qDebug() << " got config" << config;
		QString n = lash_config_get_key( config );
		//qDebug() << "  lash returnes" << static_cast<const QVariant*>( lash_config_get_value( config ) );
		QByteArray tmp( ( const char* ) lash_config_get_value( config ), lash_config_get_value_size( config ) +1 );
		qDebug() << " tmp.size()" << tmp.size();
		lash_config_destroy( config );
		QVariant v;
		{
			QDataStream in( &tmp, QIODevice::ReadOnly );
			in >> v;
		}
		_values[ n ] = v;
		qDebug() << "  evaluates to [" << n << "," << v << "]";
		emit valueChanged( n,v );
	}
	lash_send_event( _client, lash_event_new_with_type( LASH_Restore_Data_Set ) );
}

/*void qLashClient::saveToConfig( int data ) {
	qDebug() << "qLashClient::saveToConfig(" << data << ")";
	qDebug() << " sender()" << sender();
	if ( sender() && sender()->objectName().isEmpty() ) {
		lash_config_t* config = lash_config_new_with_key( sender()->objectName().toStdString().c_str() );
		lash_config_set_value_int( config, data );
		lash_send_config( _client, config );
		saveToConfigFinished();
	}
}*/

void qLashClient::setValue( QString n, QVariant v ) {
	_values[ n ] = v;
}
QVariant qLashClient::getValue( QString n ) const {
	return _values[ n ];
}

void qLashClient::timerEvent( QTimerEvent* ) {
	while ( isConnected() && lash_get_pending_event_count( _client ) > 0 ) {
		lash_event_t* event = lash_get_event( _client );
		//qDebug() << "Have event of type" << lash_event_get_type( event );

		switch ( lash_event_get_type( event ) ) {
			case LASH_Client_Name:
				qDebug() << "Event: This clients name is:" << lash_event_get_string( event );
				break;
			/*case LASH_Jack_Client_Name:
				qDebug() << "Event: This clients JACK-name is:" << lash_event_get_string( event );
				break;*/
			/*case LASH_Alsa_Client_ID:
				qDebug() << "Event: This clients Alsa-ID is:" << lash_event_get_string( event );
				break;*/
			case LASH_Save_File:
				qDebug() << "Event: Should save data into dir" << lash_event_get_string( event );
				emit saveToDir( lash_event_get_string( event ) );
				break;
			case LASH_Restore_File:
				qDebug() << "Event: Should restore data from dir" << lash_event_get_string( event );
				emit restoreFromDir( lash_event_get_string( event ) );
				break;
			case LASH_Save_Data_Set:
				qDebug() << "Event: Should save data in configs";
				emit saveToConfig();
				saveToConfigFinalize();
				break;
			case LASH_Restore_Data_Set:
				qDebug() << "Event: Should restore data from configs";
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

