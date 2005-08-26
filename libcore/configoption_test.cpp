
#include <iostream>

#include "configoption.h"

int main( int, void* )
{

	ConfigOption<int> config( "test", 5, 0 );
	IntOption intconf( "int", 42, 0 );

	std::cout << "Value: " << config.value() << std::endl;
	std::cout << "Int's value: " << intconf.value() << std::endl;

	IntOption::type tmp = intconf.value();
	std::cout << "IntOption::type tmp = " << tmp << std::endl;
	return 0;
}

