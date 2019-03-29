#ifndef __ABL_RESOURCE_H__
#define __ABL_RESOURCE_H__

// TODO: Maybe replace map with unordered_map
// Have Incomplete type issue

#include <string>
#include <optional>
#include <functional>
#include <map> 

namespace abl {
/* --------- Exception class-------- */
class resource_exception : public std::exception {
private:
   const std::string m_msg;

public:
	resource_exception(const std::string& msg);
	virtual const char* what() const throw ();
};

/* Declare for use in Resource */
template <typename value_type, typename key_type=std::string>
class Resource_ref;

/* --------- Resource class ------- */
template <typename value_type, typename key_type=std::string>
class Resource {
private:
	static std::map<key_type, Resource> resources;

	/* Points to key in map */
	const key_type *id_ptr;

	value_type* m_data_ptr = nullptr;
	std::function<value_type(void)> m_acquire_func;
	Resource(std::function<value_type(void)> acq_func);
public:
	//~Resource(){std::cerr << "Resource destructor called" << std::endl;}
	unsigned use_count{0};
	static void make_resource(const key_type& id, std::function<value_type(void)> acq_func);
	static void delete_resource(const key_type& id);

	const key_type& id();
	void update();

	/* Only could be called by resource_ref obj */
	value_type * get_data();

	static Resource_ref<value_type, key_type> get(const key_type &id);
};


/* ------- Resource_ref clas------- */
template <typename value_type, typename key_type>
class Resource_ref {
friend Resource<value_type, key_type>;
private:
	Resource<value_type, key_type> &m_origin;
	Resource_ref(Resource<value_type, key_type> &origin);

public:
	Resource_ref(const Resource_ref<value_type, key_type> &rsc);
	const value_type& operator=(const Resource_ref& rhs);

	bool operator==(const Resource_ref &rhs);
	const value_type* operator->();
	const value_type& operator*();

	const key_type& id();
	~Resource_ref();
};


/* ----- DEFINITIONS ---- */

template <typename value_type, typename key_type>
std::map<key_type, Resource<value_type, key_type>> Resource<value_type, key_type>::resources;

/* ----- Exception definitions ----- */
resource_exception::resource_exception(const std::string& msg) : m_msg(msg) { }
const char* resource_exception::what() const throw () { return m_msg.c_str(); }


/* ----- Resource definitions ----- */

template <typename value_type, typename key_type>
Resource<value_type, key_type>::Resource(std::function<value_type(void)> acq_func) : m_acquire_func(acq_func) {/*std::cerr << "Resource construcor called" << std::endl;*/}



template <typename value_type, typename key_type>
void Resource<value_type, key_type>::make_resource(const key_type& id, std::function<value_type(void)> acq_func) {
	resources.emplace(typename std::map<key_type, Resource<value_type,key_type> >::value_type(id, Resource(acq_func)));
	auto it = resources.find(id);
	it->second.id_ptr = &it->first;
}

template <typename value_type, typename key_type>
void Resource<value_type, key_type>::delete_resource(const key_type& id) {
	auto res_it = resources.find(id);
	if (res_it->second.use_count) {
		throw resource_exception("Can not delete resource: Resource busy");
	} else if (res_it == resources.end()) {
		throw resource_exception("Can not delete resource: Resource does not exist");
	} else {
		resources.erase(id);
	}
}


template <typename value_type, typename key_type>
const key_type& Resource<value_type, key_type>::id() {
	return *id_ptr;
}

template <typename value_type, typename key_type>
void Resource<value_type, key_type>::update() {
	//std::cerr << "UC: " << use_count << std::endl;
	if (use_count == 0) {
		if (m_data_ptr != nullptr) {
			delete m_data_ptr;
			m_data_ptr = nullptr;
		}
	} else /* gt 0 */ {
		if (m_data_ptr == nullptr) {
			m_data_ptr = new value_type(std::invoke(m_acquire_func));
		}
	}
}

/* Only could be called by resource_ref obj */
template <typename value_type, typename key_type>
value_type * Resource<value_type, key_type>::get_data() {
	return m_data_ptr;
}

template <typename value_type, typename key_type>
Resource_ref<value_type, key_type> Resource<value_type, key_type>::get(const key_type &id) {
	auto res_it = resources.find(id); 
	if (res_it == resources.end()) {
		throw resource_exception("Can not get resource: Resource does not exist");
	} else {
		Resource& res_ref = res_it->second;
		return Resource_ref<value_type, key_type>(res_ref);
	}
}

/* ----- Resource_ref definitions ----- */
template <typename value_type, typename key_type>
Resource_ref<value_type,key_type>::Resource_ref(Resource<value_type, key_type> &origin) : m_origin(origin) { 
	m_origin.use_count++;
	m_origin.update();
}

// copy constructor
template <typename value_type, typename key_type>
Resource_ref<value_type, key_type>::Resource_ref(const Resource_ref<value_type, key_type> &rsc) : Resource_ref(rsc.m_origin) {};

// copy assignment operator
template <typename value_type, typename key_type>
const value_type& Resource_ref<value_type, key_type>::operator=(const Resource_ref& rhs) {
	if (this != &rhs) { 
		*m_origin = rhs.m_origin;
	}
}

template <typename value_type, typename key_type>
const value_type* Resource_ref<value_type, key_type>::operator->() { 
	return m_origin.get_data(); 
}

template <typename value_type, typename key_type>
const value_type& Resource_ref<value_type, key_type>::operator*() { return *m_origin.get_data(); }

template <typename value_type, typename key_type>
const key_type& Resource_ref<value_type, key_type>::id() { 
	return m_origin.id();
}

template <typename value_type, typename key_type>
Resource_ref<value_type, key_type>::~Resource_ref() {
	m_origin.use_count--;
	m_origin.update();
}

template <typename value_type, typename key_type>
bool Resource_ref<value_type, key_type>::operator==(const Resource_ref &rhs) {
	return this->m_origin.id() == rhs.m_origin.id();
}

} // namespace abl

#endif // ABL_RESOURCE_H
