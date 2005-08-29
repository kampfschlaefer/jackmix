
#include "configoption.h"
#include "configoption.moc"

#include <iostream>

void ConfigOption::newChild( QString n, QVariant v ) {
	if ( _type == QVariant::Invalid )
		addChild( new ConfigOption( n, v ) );
}
void ConfigOption::newChild( QString n, QVariant::Type t ) {
	qDebug( "ConfigOption::newChild( QString %s, QVariant::Type %s )", n.latin1(), QVariant::typeToName( t ) );
	if ( _type == QVariant::Invalid )
		addChild( new ConfigOption( n, t ) );
	qDebug( "New child has type %s", QVariant::typeToName( _childs[ n ]->type() ) );
}
void ConfigOption::addChild( ConfigOption* n ) {
	if ( _type == QVariant::Invalid )
		_childs[ n->_name ] = n;
}
void ConfigOption::removeChild( ConfigOption* n ) {
	_childs.remove( n->_name );
}

ConfigOption* ConfigOption::getOption( QString n ) {
	while ( n[ 0 ]==QChar( '/' ) ) n.remove( 0,1 );
//	qDebug( "ConfigOption::getOption( QString %s )", n.latin1() );
	int levels = n.contains( "/" ); // + 1
	if ( n.section( "/",0,0 ) == _name ) {
		// The first part is our name, go on...
//		qDebug( "levels = %i", levels );
		if ( levels == 0 )
			return this;
		else {
			QString tmp = n.section( "/", 1,1 );
//			qDebug( "tmp = %s", tmp.latin1() );
			ConfigOption* ret = 0;
			for ( ChildList::Iterator it=_childs.begin(); it!=_childs.end() && ret == 0; ++it ) {
				ret = ( *it )->getOption( tmp );
			}
			return ret;
		}
	}
	else // first part isn't our name, return 0
		return 0;
}

void ConfigOption::value( QVariant n ) {
	static bool _inupdate = false;
	if ( !_inupdate && _type!=QVariant::Invalid ) {
		if ( n.canCast( _type ) )
			_value = n;
		_inupdate = true;
		emit changed( _value );
		if ( _value.canCast( QVariant::Int ) ) emit changed( _value.toInt() );
		if ( _value.canCast( QVariant::Double ) ) emit changed( _value.toDouble() );
		if ( _value.canCast( QVariant::String ) ) emit changed( _value.toString() );
		_inupdate = false;
	} else
		qWarning( "ConfigOption::value() Either in inupdate or wrong datatype! (_inupdate=%i,_type=%s[%s|%s])", _inupdate, QVariant::typeToName( n.type() ), QVariant::typeToName( _type ), QVariant::typeToName( _value.type() ) );
}



ConfigOption::operator QVariant( void ) {
	Q_ASSERT( _value.isValid() );
	return _value;
}

void ConfigOption::debugPrint( int level ) {
//	std::cout << "ConfigOption::debugPrint( int " << level << " )" << std::endl;
	for ( int i=0; i<level; ++i )
		std::cout << " + ";
//	std::cout << _name.latin1() << "(" << QVariant::typeToName( _type ) << ")" << std::endl;
	if ( _value.isValid() )
		std::cout << _name.latin1() << "(" << QVariant::typeToName( _type ) << ") = " << _value.toString() << std::endl;
	else {
		std::cout << _name.latin1() << "(Invalid)" << /*"(" << QVariant::typeToName( _type ) << ")" <<*/ std::endl;
		for ( ChildList::Iterator it = _childs.begin(); it!=_childs.end(); ++it )
			( *it )->debugPrint( level + 1 );
	}
}

QDataStream& operator<<( QDataStream& s, const ConfigOption* c ) {
	s << c->_name << c->_value;
	if ( c->_type == QVariant::Invalid ) {
		s << c->childrenCount();
		for ( ConfigOption::ChildList::ConstIterator it= c->_childs.begin(); it!=c->_childs.end(); ++it ) {
//			it.data()->debugPrint();
			s << it.data();
		}
	}
	return s;
}
QDataStream& operator>>( QDataStream& s, ConfigOption* c ) {
	s >> c->_name >> c->_value;
	if ( c->_type == QVariant::Invalid ) {
		int count=0;
		s >> count;
/*		for ( int i=0; i<count; ++i ) {
			ConfigOption* tmp = new ConfigOption( "" );
			s >> tmp;
			c->addChild( tmp );
		}*/
		qDebug( "Said to be %i childs", count );
	}
	return s;
}

