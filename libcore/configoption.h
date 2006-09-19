#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QList>

class ConfigOption : public QObject
{
	Q_OBJECT
	public:
		enum OverwriteMode { OnlyNew, ValuesandNew, Everything };
		QVariant::Type type() { return _type; }
		ConfigOption( QString n, QVariant::Type t = QVariant::Invalid, QObject* p=0 )
			: QObject( p ), _type( t ) , _name( n )
			{
//				qDebug( "ConfigOption::ConfigOption( QString %s, QVariant::Type %s )", n.latin1(), QVariant::typeToName( t ) );
//				qDebug( " Internals: _name = %s  _type = %s", _name.latin1(), QVariant::typeToName( type() ) );
			}
		ConfigOption( QString n, QVariant v, QObject* p=0 )
			: QObject( p ), _type( v.type() ) , _name( n ), _value( v )
			{ }
		ConfigOption( QDataStream&, QObject* =0 );
		virtual ~ConfigOption();

		void newChild( QString, QVariant );
		void newChild( QString, QVariant::Type = QVariant::Invalid );
		void addChild( ConfigOption* );
		void removeChild( ConfigOption* );
		int childrenCount() const { return _childs.count(); }

		ConfigOption* getOption( QString );

		QVariant value() const { return _value; }

		void fromDataStream( QDataStream&, OverwriteMode );

//		operator QVariant( void );
		friend QDataStream& operator<<( QDataStream&, ConfigOption* );
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
		typedef QList <ConfigOption*> ChildList;
		ChildList _childs;
};

QDataStream& operator<<( QDataStream&, ConfigOption* );
QDataStream& operator>>( QDataStream&, ConfigOption* );

#endif // CONFIGOPTION_H

