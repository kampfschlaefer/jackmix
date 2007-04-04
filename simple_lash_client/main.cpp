
#include <QtGui/QtGui>

#include <lash/lash.h>
//#include <lash/

int main( int argc, char** argv ) {
	qDebug() << "simple_lash_client starting";

	lash_event_t *event;
	lash_config_t *config;
	lash_client_t *client;
	char *client_name;
	//int err;
	//int done =0;

	client_name = "LASH QPushButton...";

	client = lash_init( lash_extract_args( &argc,&argv ), "LASH QPushButton", LASH_Config_Data_Set /*| LASH_Config_File*/, LASH_PROTOCOL( 2,0 ) );
	if ( !client )
		return 1;

	event = lash_event_new_with_type( LASH_Client_Name );
	lash_event_set_string( event, client_name );
	lash_send_event( client, event );

	QApplication qapp( argc, argv );

	QPushButton* btn = new QPushButton( "Button" );
	btn->setCheckable( true );

	btn->show();

	event = lash_get_event( client );
	while ( event ) {

		//btn->setText( QString( "%1\nGot event of type %2" ).arg( btn->text() ).arg( lash_event_get_type( event ) ) );

		if ( lash_event_get_type( event ) == LASH_Restore_Data_Set ) {
			//btn->setText( QString( "%1\nShould restore data" ).arg( btn->text() ) );

			config = lash_get_config( client );

			int i = lash_config_get_value_int( config );
			btn->setChecked( i );

			lash_config_destroy( config );
		}

		event = lash_get_event( client );
	}

	int ret = qapp.exec();

	config = lash_config_new_with_key( "buttonstate" );
	lash_config_set_value_int( config, btn->isChecked() );
	lash_send_config( client, config );

	qDebug() << "simple_lash_client finishing with return-code" << ret;
	return ret;
}

