#ifndef CONFIGOPTION_H
#define CONFIGOPTION_H

#include <string>
#include <list>

template<class T> class ConfigOption
{
	public:
		typedef T type;
		ConfigOption( std::string name, T value, ConfigOption* parent )
			: _parent( parent )
			, _name( name )
			, _value( value )
			{
			}
		virtual ~ConfigOption() {}

		virtual void addChild( ConfigOption* ) {}
		virtual void removeChild( ConfigOption* ) {}
		virtual int childrenCount() { return 0; }

		virtual ConfigOption* getOption( std::string ) { return 0; }

		virtual T value() { return _value; }
		virtual void setValue( T n ) { _value = n; }
	private:
		ConfigOption* _parent;
		std::string _name;
		T _value;
};

typedef ConfigOption<int> IntOption;

class ConfigOptionList : public ConfigOption<int>
{
	public:
		ConfigOptionList( std::string name, ConfigOption* parent );
		~ConfigOptionList();

		void addChild( ConfigOption* );
		void removeChild( ConfigOption* );

		int childrenCount();
	private:
		std::list<ConfigOption*> _childs;
};

/*template<class T> class ConfigOption : public ConfigOptionBase
{
	public:
		ConfigOption( std::string name, T value, ConfigOptionBase* parent ) : ConfigOptionBase( name, parent ), _value( value )
		{
		}
		~ConfigOption() {}

	private:
		T _value;
};*/

#endif // CONFIGOPTION_H

