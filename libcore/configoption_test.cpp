
#include <iostream>

//#define private public
#include "configoption.h"
#include <QtCore/QFile>
#include <QtCore/QDataStream>

int main( int, void* )
{

	ConfigOption* _config = new ConfigOption( "test", QVariant::Invalid );
	ConfigOption c_int( "int", QVariant::Int );
	c_int.value( 42 );
	_config->addChild( &c_int );
	_config->newChild( "double", QVariant::Double );
	_config->newChild( "string", "test" );
	_config->newChild( "dir" );
	qDebug( "main.cpp: newest type is %s", QVariant::typeToName( _config->getOption( "test/dir" )->type() ) );
	_config->getOption( "test/dir" )->newChild( "first", QVariant::Int );
	qDebug( "main.cpp: newest type is %s", QVariant::typeToName( _config->getOption( "test/dir/first" )->type() ) );
	_config->getOption( "test/dir" )->newChild( "second" );
	_config->getOption( "test/dir/first" )->value( 2 );

	_config->getOption( "test/double" )->value( 5.4 );
	_config->debugPrint();
	_config->getOption( "test/double" )->value( 6.0 );
	_config->getOption( "test/double" )->debugPrint();

	QFile file( ".test.out" );
	if ( file.open( QIODevice::WriteOnly ) ) {
		QDataStream stream( &file );
		stream << _config;
		file.close();
	}
	delete _config;
	_config = 0;

	qDebug( "\n\nTesting disk-IO\nLoading a config...\n\n" );

	ConfigOption* test = 0;
	QFile file2( ".test.out" );
	if ( file.open( QIODevice::ReadOnly ) ) {
		QDataStream stream( &file );
//		stream >> test;
		test = new ConfigOption( stream );
		file.close();
	}
	test->debugPrint();


/*	ConfigOption* test2 = new ConfigOption( "test2" );
	test2->newChild( "int", QVariant::Int );

	std::cout << "Bla 1" << std::endl;
	test2->debugPrint();
	test2->getOption( "test2/int" )->value( 2 );
	test2->debugPrint();
	std::cout << "Bla 2" << std::endl;*/

	std::cout << ".\n ConfigOption_Test finished!" << std::endl;
	return 0;
}

