#include <iostream>
#include "../../include/sgf.hpp"

class Foo {
public:
	std::string s;
	Foo(const std::string &s) : s(s) {
		std::cout << "Foo constructor called" << std::endl;
	}

	static Foo load(const std::string &s) {
		return Foo(s);
	}
};

void some_func() {
	std::cout << "From some_func: " << sgf::resource<Foo>::get("my_resource").s << std::endl;
}

int main()
{
        sgf::resource<Foo>::acquire("my_resource", Foo("One-foo"));
        const Foo &f = sgf::resource<Foo>::get("my_resource");
        std::cout << f.s << std::endl;

        try {
        	sgf::resource<Foo>::acquire("my_resource", Foo("Two-foo"));
        } catch(const sgf::resource_exception &e) {
        	// Resource already acquired exception
        	// could be throwed
        	std::cout << e.what() << std::endl;
        }

        const Foo &f1 = sgf::resource<Foo>::get("my_resource");

        std::cout << "Should not: " << sgf::resource<Foo>::is_acquired("randomname") << std::endl;

        std::cout << f.s << " " << f1.s << std::endl;
        std::cout << &f << " " << &f1 << std::endl;

        std::vector<std::tuple<std::string, Foo>> vec = {{"me", Foo("me")}, {"you", Foo("you")}};

        for (const auto& [k, v] : vec) {
        	try {
        		sgf::resource<Foo>::acquire(k, v);
        	} catch (const sgf::resource_exception &e) {
        		std::cout << e.what();
        	}
        }

        std::cout << sgf::resource<Foo>::get("me").s << std::endl;
        some_func();

        /* Lazy eval */
        sgf::resource<Foo>::lazy_acquire("lazy_res", []() { return Foo("lazy_res"); });
        std::cout << "Is acquired: " << sgf::resource<Foo>::is_acquired("lazy_res") << std::endl;
        
        std::cout << sgf::resource<Foo>::get("lazy_res").s << std::endl;

        std::cout << "Is acquired: " << sgf::resource<Foo>::is_acquired("lazy_res") << std::endl;

      	sgf::resource<Foo>::lazy_acquire("lazyone", []() {return Foo("lazyone");});
	std::cout << "Already lazely acquired: lazyone" << std::endl;
	std::cout << sgf::resource<Foo>::get("lazyone").s << std::endl;
	std::cout << sgf::resource<Foo>::get("lazyone").s << std::endl;

	/* Bind */
	/* _ could be placeholder for unbounded element */
	/* To pass args by ref use std::ref or std::cref */
	auto bf = std::bind(&Foo::load, "binded");
	sgf::resource<Foo>::lazy_acquire("bf", bf);
	std::cout << sgf::resource<Foo>::get("bf").s << std::endl;

	std::vector<std::string> names{"bind1", "bind2", "bind3", "bind4"};
	for (const auto &name : names) {
		auto bn = std::bind(&Foo::load, name);
		sgf::resource<Foo>::lazy_acquire(name, bn);
	}

	for (const auto &name : names) {
		std::cout << sgf::resource<Foo>::get(name).s << std::endl;
	}
}
