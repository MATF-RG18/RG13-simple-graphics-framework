#include "../../include/sgf.hpp"
#include <iostream>

class Foo {
public:
	std::string s;
	Foo(const std::string &s) : s(s) {
		std::cout << "Foo constructor called" << std::endl;
	}

	~Foo() {
		std::cout << "Foo destructor called" << std::endl;
	}

	static Foo load(std::string name) {
		return Foo(name);
	}
};


int main()
{
	// Here is being maked and could be acquired after this
	// First argument is name, second argument is func: how to acquire it */
	sgf::Resource<Foo>::make_resource("MyResource", std::bind(Foo::load, "content of resource"));

	{ // block is lifespan for references inside

		// Here is resource being acquired for first time 
		sgf::Resource_ref<Foo> rsc = sgf::Resource<Foo>::get("MyResource");
		// You can use operator '->'
		std::cout << rsc->s << std::endl;
		
		// You can use operator '*'
		std::cout << (*rsc).s << std::endl;

		sgf::Resource_ref<Foo> rsc1 = sgf::Resource<Foo>::get("MyResource");
		std::cout << rsc1->s << std::endl;

		// Copy constructor (both ref-s point to same resource)
		sgf::Resource_ref<Foo> rsc2(rsc1);
		std::cout << rsc2->s << std::endl;

		// Copy assignment operator (both ref-s point to same resource)
		sgf::Resource_ref<Foo> rsc3 = rsc1;
		std::cout << rsc2->s << std::endl;

		std::cout << "addr(rsc1): " << &rsc1 << ", adr(rsc2): " << &rsc2 << ", eq: " << (rsc2 == rsc3) << std::endl;
		std::cout << "addr(rsc2): " << &rsc2 << ", adr(rsc3): " << &rsc3 << ", eq: " << (rsc2 == rsc3) << std::endl;
		
		/* You can get resource id also */
		std::cout << "id: " << rsc2.id() << std::endl;

	std::cout << "Still acquired" << std::endl;
	} // Here is unacquired since no one refers to it
	std::cout << "Unacquired" << std::endl;

	/* Again acquiring */
	sgf::Resource_ref<Foo> rsc = sgf::Resource<Foo>::get("MyResource");
	std::cout << rsc->s << std::endl;

	sgf::Resource_ref<Foo> rsc2 = sgf::Resource<Foo>::get("MyResource");
	std::cout << rsc2->s << std::endl;
	
	// This deletes even possibility to be acquired
	// Resource<Foo>::delete_resource("MyResource"); // not needed
	// From now, next line would result in error */
	// Resource_ref<Foo> rsc2 = Resource<Foo>::get("MyResource");

	return 0;

} // Again unacquired // and deleted
