
#include "configoption.h"
#include "configoption.moc"

#include <iostream>

ConfigOption::ConfigOption( QDataStream& s, QObject* p ) : QObject( p )/*, _value( s )*/, _value( QVariant() ) {
//	qDebug( "ConfigOption::ConfigOption( QDataStream )" );
	s >> _name >> _value;
	_type = _value.type();
//	qDebug( " _name=%s, _value=%s", _name.toStdString().c_str(), _value.toString().toStdString().c_str() );
	if ( type() == QVariant::Invalid ) {
		int count;
		s >> count;
//		qDebug( "Found %i childs", count );
		for ( int i=0; i<count; ++i )
			addChild( new ConfigOption( s, this ) );
//		qDebug( "I have now %i childs", _childs.count() );
	}
}

ConfigOption::~ConfigOption() {
	qDebug( "(%s) ConfigOption::~ConfigOption()", _name.toStdString().c_str() );
}

void ConfigOption::newChild( QString n, QVariant v ) {
	if ( type() == QVariant::Invalid )
		addChild( new ConfigOption( n, v, this ) );
}
void ConfigOption::newChild( QString n, QVariant::Type t ) {
//	qDebug( "ConfigOption::newChild( QString %s, QVariant::Type %s )", n.toStdString().c_str(), QVariant::typeToName( t ) );
	if ( type() == QVariant::Invalid )
		addChild( new ConfigOption( n, t, this ) );
//	qDebug( "New child has type %s \nI (%s) have now %i childs.", QVariant::typeToName( _childs[ n ]->type() ), _name.toStdString().c_str(), _childs.size() );
}
void ConfigOption::addChild( ConfigOption* n ) {
	if ( _type == QVariant::Invalid )
		_childs.append( n );
}
void ConfigOption::removeChild( ConfigOption* /*n*/ ) {
//	_childs.remove( n->_name ); // XXX
}

ConfigOption* ConfigOption::getOption( QString n ) {
	while ( n[ 0 ]==QChar( '/' ) ) n.remove( 0,1 );
//	qDebug( "ConfigOption::getOption( QString %s )", n.toStdString().c_str() );
	int levels = n.count( "/" ); // + 1
	if ( n.section( "/",0,0 ) == _name ) {
		// The first part is our name, go on...
		if ( levels == 0 )
			return this;
		else {
			QString tmp = n.section( "/", 1,levels );
			ConfigOption* ret = 0;
			for ( ConfigOption* it=_childs.first(); it && ret == 0; it++ ) {
				ret = it->getOption( tmp );
			}
			return ret;
		}
	}
	else // first part isn't our name, return 0
		return 0;
}

void ConfigOption::value( QVariant n ) {
	static bool _inupdate = false;
	if ( !_inupdate && type() != QVariant::Invalid ) {
		if ( n.canConvert( type() ) )
			_value = n;
		_inupdate = true;
		emit changed( _value );
		if ( _value.canConvert( QVariant::Int ) ) emit changed( _value.toInt() );
		if ( _value.canConvert( QVariant::Double ) ) emit changed( _value.toDouble() );
		if ( _value.canConvert( QVariant::String ) ) emit changed( _value.toString() );
		_inupdate = false;
	} else
		qWarning( "ConfigOption::value() Either in inupdate or wrong datatype! (_inupdate=%i,_type=%s[%s|%s])", _inupdate, QVariant::typeToName( n.type() ), QVariant::typeToName( type() ), QVariant::typeToName( _value.type() ) );
}


void ConfigOption::fromDataStream( QDataStream& s, OverwriteMode m ) {
	QString new_name;
	QVariant new_value;
	s >> new_name >> new_value;
	if ( m == Everything ) {
		_name = new_name;
		_value = new_value;
		_type = _value.type();
	} else if ( m == ValuesandNew && new_name == _name && new_value.type() == _type ) {
		_value = new_value;
	}
	int childcount;
	s >> childcount;
	/* TODO
	 */
}

//X ConfigOption::operator QVariant( void ) {
//X 	Q_ASSERT( _value.isValid() );
//X 	return _value;
//X }

void ConfigOption::debugPrint( int level ) {
//	std::cout << "ConfigOption::debugPrint( int " << level << " )" << std::endl;
	for ( int i=0; i<level; ++i )
		std::cout << " + ";
//	std::cout << _name.toStdString().c_str() << "(" << QVariant::typeToName( _type ) << ")" << std::endl;
	if ( type() != QVariant::Invalid ) {
		std::cout << _name.toStdString().c_str() << "(" << QVariant::typeToName( type() ) << ") = '";
		if ( _value.canConvert( QVariant::String ) )
			std::cout << _value.toString().toStdString().c_str();
		else
			std::cout << "[can't cast " << QVariant::typeToName( _value.type() ) << " to string]";
		std::cout << "'" << std::endl;
	} else {
		std::cout << _name.toStdString().c_str() << "(Invalid)" << std::endl;
		for ( ConfigOption* it = _childs.first(); it; it++ )
			it->debugPrint( level + 1 );
	}
}

QDataStream& operator<<( QDataStream& s, ConfigOption* c ) {
	s << c->_name << c->_value;
	if ( c->_type == QVariant::Invalid ) {
		s << c->childrenCount();
		for ( ConfigOption* it= c->_childs.first(); it; it++ )
			s << it;
	}
	return s;
}
QDataStream& operator>>( QDataStream& s, ConfigOption* c ) {
#if 0
	s >> c->_name >> c->_value;
	if ( c->_type == QVariant::Invalid ) {
		int count=0;
		s >> count;
		// TODO
/*		for ( int i=0; i<count; ++i ) {
			ConfigOption* tmp = new ConfigOption( "" );
			s >> tmp;
			c->addChild( tmp );
		}*/
		qDebug( "Said to be %i childs", count );
	}
#endif
	return s;
}

