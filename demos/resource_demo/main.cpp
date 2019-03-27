#include <iostream>
#include "../../include/sgf.hpp"

class Foo {
public:
	std::string s;
	Foo(const std::string &s) : s(s) {
		std::cout << "Foo constructor called" << std::endl;
	}
};

void some_func() {
	std::cout << "From some_func: " << abl::resource<Foo>::get("my_resource").s << std::endl;
}

int main()
{
        abl::resource<Foo>::acquire("my_resource", Foo("One-foo"));
        const Foo &f = abl::resource<Foo>::get("my_resource");
        std::cout << f.s << std::endl;

        try {
        	abl::resource<Foo>::acquire("my_resource", Foo("Two-foo"));
        } catch(const abl::resource_exception &e) {
        	// Resource already acquired exception
        	// could be throwed
        	std::cout << e.what() << std::endl;
        }

        const Foo &f1 = abl::resource<Foo>::get("my_resource");

        std::cout << "Should not: " << abl::resource<Foo>::is_acquired("randomname") << std::endl;

        std::cout << f.s << " " << f1.s << std::endl;
        std::cout << &f << " " << &f1 << std::endl;

        std::vector<std::tuple<std::string, Foo>> vec = {{"me", Foo("me")}, {"you", Foo("you")}};

        for (const auto& [k, v] : vec) {
        	try {
        		abl::resource<Foo>::acquire(k, v);
        	} catch (const abl::resource_exception &e) {
        		std::cout << e.what();
        	}
        }

        std::cout << abl::resource<Foo>::get("me").s << std::endl;
        some_func();

        /* Lazy eval */
        abl::resource<Foo>::lazy_acquire("lazy_res", []() { return Foo("lazy_res"); });
        std::cout << "Is acquired: " << abl::resource<Foo>::is_acquired("lazy_res") << std::endl;
        
        std::cout << abl::resource<Foo>::get("lazy_res").s << std::endl;

        std::cout << "Is acquired: " << abl::resource<Foo>::is_acquired("lazy_res") << std::endl;

      	abl::resource<Foo>::lazy_acquire("lazyone", []() {return Foo("lazyone");});
	std::cout << "Already lazely acquired: lazyone" << std::endl;
	std::cout << abl::resource<Foo>::get("lazyone").s << std::endl;
	std::cout << abl::resource<Foo>::get("lazyone").s << std::endl;

}
