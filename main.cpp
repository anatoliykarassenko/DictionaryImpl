#include <iostream>
#include "string.h"
#include "dictionaryImpl.hpp"

int main()
{
    try
	{
		DictionaryImpl<int, std::string> dict;
		dict.Set(0, "Name 0");
		dict.Set(1, "Name 1");
		dict.Set(2, "Name 2");
		dict.Set(3, "Name 3");
		std::cout << std::boolalpha << dict.IsSet(0) << ' ' << dict.Get(0) << '\n';
		std::cout << std::boolalpha << dict.IsSet(1) << ' ' << dict.Get(1) << '\n';
		std::cout << std::boolalpha << dict.IsSet(2) << ' ' << dict.Get(2) << '\n';
	}
	catch (const NotFoundExceptionImpl<int>& except)
	{
		std::cerr << except.what() << '\n';
		std::cerr << except.GetKey() << '\n';
	}
}
