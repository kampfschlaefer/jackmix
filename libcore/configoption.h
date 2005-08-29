#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H

#include <qobject.h>
#include <qstring.h>
#include <qvariant.h>
#include <qmap.h>

class ConfigOption : public QObject
{
	Q_OBJECT
	public:
		QVariant::Type type() { return _type; }
		ConfigOption( QString n, QVariant::Type t = QVariant::Invalid )
			: _type( t ) , _name( n )
			{
//				qDebug( "ConfigOption::ConfigOption( QString %s, QVariant::Type %s )", n.latin1(), QVariant::typeToName( t ) );
//				qDebug( " Internals: _name = %s  _type = %s", _name.latin1(), QVariant::typeToName( type() ) );
			}
		ConfigOption( QString n, QVariant v )
			: _type( v.type() ) , _name( n ), _value( v )
			{ }
		ConfigOption( QDataStream& s ) {
			qDebug( "ConfigOption::ConfigOption( QDataStream )" );
			s >> _name >> _value;
			_type = _value.type();
			if ( type() == QVariant::Invalid ) {
				int count;
				s >> count;
				qDebug( "Found %i childs", count );
				for ( int i=0; i<count; ++i ) {
					addChild( new ConfigOption( s ) );
				}
				qDebug( "I have now %i childs", _childs.count() );
			}
		}
		virtual ~ConfigOption();

		void newChild( QString, QVariant );
		void newChild( QString, QVariant::Type = QVariant::Invalid );
		void addChild( ConfigOption* );
		void removeChild( ConfigOption* );
		int childrenCount() const { return _childs.count(); }

		ConfigOption* getOption( QString );

		QVariant value() const { return _value; }

//		operator QVariant( void );
		friend QDataStream& operator<<( QDataStream&, const ConfigOption* );
		friend QDataStream& operator>>( QDataStream&, ConfigOption* );

		void debugPrint() { debugPrint( 0 ); }

	public slots:
		void value( QVariant );
		void value( int n ) { value( QVariant( n ) ); }
		void value( double n ) { value( QVariant( n ) ); }
		void value( QString n ) { value( QVariant( n ) ); }
	signals:
		void changed( QVariant );
		void changed( int );
		void changed( double );
		void changed( QString );

	private:
		void debugPrint( int level );
		QVariant::Type _type;
		QString _name;
		QVariant _value;
		typedef QMap <QString,ConfigOption*> ChildList;
		ChildList _childs;
};

QDataStream& operator<<( QDataStream&, const ConfigOption* );
QDataStream& operator>>( QDataStream&, ConfigOption* );

#endif // CONFIGOPTION_H

