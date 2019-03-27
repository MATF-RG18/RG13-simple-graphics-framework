#include <iostream>
#include <filesystem>
#include "../../include/sgf.hpp"

class Foo {
public:
	std::string s;
	Foo(const std::string &s) : s(s) {}
};

int main()
{
	//sgf::named_resource<Foo>::give_name("resources.cpp", "MojResurs");
	const Foo &f = sgf::get_resource<Foo>("resources.cpp");
	sgf::named_resource<Foo>::acquire("resources.cpp", "MyResource");
	const Foo &f1 = sgf::get_resource<Foo>("resources.cpp");
	
	std::cout << "Should: " << sgf::named_resource<Foo>::is_acquired_by_name("MyResource") << std::endl;

	std::cout << "Should not: " << sgf::named_resource<Foo>::is_acquired_by_name("MyResource") << std::endl;
	std::cout << &f << " " << f1.s << " " <<  std::endl;
	sgf::named_resource<Foo>::erase_by_name("MyResource");

	const Foo &f3 = sgf::get_resource_by_name<Foo>("MyResource");
	std::cout << f.s << " " << f1.s << " " << f3.s << std::endl;

	return 0;
}
