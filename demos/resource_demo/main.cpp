#include "../../include/sgf.hpp"
#include <iostream>

using namespace sgf;

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
	// if you dont want automatic resource management, pass false as third argument 
	// if you pass false, use flush for flushing resource out of ram memory
	// When you use get next time it will be loaded back to ram
	Resource<Foo>::make("MyResource", std::bind(Foo::load, "content of resource")/*, false*/ );

	/* You could use manual acquire (to ensure resource loads to RAM before first get) *
	 * Resource would be loaded to ram anyways with first Resource<Foo>::get() call */
	Resource<Foo>::acquire("MyResource");
	std::cout << "Acquired" << std::endl;

	/* You can use flush if no one is using resource, to unload it from RAM */
	Resource<Foo>::flush("MyResource");
	std::cout << "Flushed" << std::endl;

	{ // block is lifespan for references inside

		// Here is resource being acquired (loaded in RAM) since
		// there is no one using it before and it is not already loaded
		Resource_ref<Foo> rsc = Resource<Foo>::get("MyResource");
		// You can use operator '->'
		std::cout << rsc->s << std::endl;
		
		// You can use operator '*'
		std::cout << (*rsc).s << std::endl;

		Resource_ref<Foo> rsc1 = Resource<Foo>::get("MyResource");
		std::cout << rsc1->s << std::endl;

		// Copy ;
		Resource_ref<Foo> rsc2(rsc1);
		std::cout << rsc2->s << std::endl;

		// Copy assignment operator (both ref-s point to same resource)
		Resource_ref<Foo> rsc3 = rsc1;
		std::cout << rsc2->s << std::endl;

		std::cout << "addr(rsc1): " << &rsc1 << ", adr(rsc2): " << &rsc2 << ", eq: " << (rsc2 == rsc3) << std::endl;
		std::cout << "addr(rsc2): " << &rsc2 << ", adr(rsc3): " << &rsc3 << ", eq: " << (rsc2 == rsc3) << std::endl;
		
		/* You can get resource id also */
		std::cout << "id: " << rsc2.id() << std::endl;

	std::cout << "Still acquired" << std::endl;
	} // Here is flushed since no one refers to it

	std::cout << "Unacquired" << std::endl;

	/* Again acquiring */
	{
		Resource_ref<Foo> rsc = Resource<Foo>::get("MyResource");
		std::cout << rsc->s << std::endl;
		Resource_ref<Foo> rs1 = Resource<Foo>::get("MyResource");
		std::cout << rsc->s << std::endl;
	}

	//Resource_ref<Foo> rsc2 = Resource<Foo>::get("MyResource");
	//std::cout << rsc2->s << std::endl;
	
	// This deletes even possibility to be acquired
	// Resource<Foo>::remove("MyResource"); // not needed
	// From now, next line would result in error */
	// Resource_ref<Foo> rsc2 = Resource<Foo>::get("MyResource");
	
	std::cout << "Bye bye" << std::endl;

	return 0;

} // Again unacquired // and deleted
