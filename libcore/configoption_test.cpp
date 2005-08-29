
#include <iostream>

//#define private public
#include "configoption.h"
#include <qfile.h>
#include <qdatastream.h>

int main( int, void* )
{

	ConfigOption _config( "test", QVariant::Invalid );
	ConfigOption c_int( "int", QVariant::Int );
	c_int.value( 42 );
	_config.addChild( &c_int );
	_config.newChild( "double", QVariant::Double );
	_config.newChild( "string", "test" );
	_config.newChild( "dir" );
	_config.getOption( "test/dir" )->newChild( "first", QVariant::Int );
	qDebug( "main.cpp: newest type is %s", QVariant::typeToName( _config.getOption( "test/dir/first" )->type() ) );
	_config.getOption( "test/dir" )->newChild( "second" );
	_config.getOption( "test/dir/first" )->value( 2 );

	_config.getOption( "test/double" )->value( 5.4 );
	_config.debugPrint();
	_config.getOption( "test/double" )->value( 6.0 );
	_config.getOption( "test/double" )->debugPrint();

	std::cout << _config.getOption( "test" ) << std::endl;
	std::cout << _config.getOption( "test/int" ) << std::endl;
	std::cout << _config.getOption( "test/dir/first" ) << std::endl;
//	std::cout << _config.getOption( "//settings/test" ) << std::endl;
//	std::cout << _config.getOption( "settings/test/int" ) << std::endl;

	QFile file( ".test.out" );
	if ( file.open( IO_WriteOnly ) ) {
		QDataStream stream( &file );
		stream << &_config;
		file.close();
	}

	ConfigOption* test = 0;
	QFile file2( ".test.out" );
	if ( file.open( IO_ReadOnly ) ) {
		QDataStream stream( &file );
//		stream >> test;
		test = new ConfigOption( stream );
		file.close();
	}
//	std::cout << "Int's value: " << intconf.value() << std::endl;

//	IntOption::type tmp = intconf.value();
//	std::cout << "IntOption::type tmp = " << tmp << std::endl;
	return 0;
}

