#ifndef __ABL_RESOURCE_H__
#define __ABL_RESOURCE_H__

#include <string>
#include <unordered_map>
#include <functional>
#include <variant>

namespace abl {

template <typename value_type, typename key_type=std::string>
class resource {
private:
	resource() = delete;
	static std::unordered_map<key_type, std::variant<value_type, std::function<value_type(void)>>> resources;

public:
	static const value_type& get(const key_type &);
	static bool is_acquired(const key_type &);
	static void erase(const key_type &);
	static void acquire(const key_type&, const value_type&);
	static void lazy_acquire(const key_type&, const std::function<value_type(void)>&);
};

class resource_exception : public std::exception {
private:
   const std::string m_msg;

public:
	resource_exception(const std::string& msg) : m_msg(msg) { }
	virtual const char* what() const throw () { return m_msg.c_str(); }
};

/* Initialize resources map */
template <typename value_type, typename key_type>
std::unordered_map<key_type, std::variant<value_type, std::function<value_type(void)>>> resource<value_type,key_type>::resources;


template <typename value_type, typename key_type>
const value_type& resource<value_type, key_type>::get(const key_type &key) {

	if (!is_acquired(key))
		throw resource_exception("Resource with provided key is not acquired");
	try {
		std::get<value_type>(resources.at(key));
	} catch (const std::bad_variant_access&) {
		auto f = std::get<std::function<value_type(void)>>(resources.at(key));
		const value_type& result = std::invoke(f);
		erase(key);
		acquire(key, std::move(result));
	}

	return std::get<value_type>(resources.at(key));
}

template <typename value_type, typename key_type>
void resource<value_type, key_type>::acquire(const key_type &key, const value_type &val) {
	if (!is_acquired(key)) {
	        resources.insert(typename std::unordered_map<key_type, value_type>::value_type(key, val));
	} else {
		throw resource_exception("Resource already acquired.");
	}
}

template <typename value_type, typename key_type>
void resource<value_type, key_type>::lazy_acquire(const key_type& key, const std::function<value_type(void)> &acq_func) {
	resources.insert(typename std::unordered_map<key_type, std::function<value_type(void)>>::value_type(key, acq_func));
}

template <typename value_type, typename key_type>
bool resource<value_type, key_type>::is_acquired(const key_type &key) {
	if (resources.find(key) != resources.end()) 
		return true;
	return false;
}

template <typename value_type, typename key_type>
void resource<value_type, key_type>::erase(const key_type &key) {
	resources.erase(key);
}

}

#endif
