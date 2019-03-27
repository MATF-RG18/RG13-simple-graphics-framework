#ifndef __SGF_RESOURCE_H__
#define __SGF_RESOURCE_H__

#include <filesystem>
#include <string>
#include <map>

namespace abl {

template <typename T>
class resource {
private:
	resource() = delete;
	static std::map<std::filesystem::path, T> resources;

public:
	static const T& get(const std::string &res_path);
	static void acquire(const std::string &res_path);
	static bool is_acquired(const std::string &res_path);
	static void erase(const std::string &res_path);
};


/* Initialize resources map */
template <class T>
std::map<std::filesystem::path, T> resource<T>::resources;


template <typename T>
const T& resource<T>::get(const std::string &res_path) {
	std::filesystem::path res_abs_path(std::filesystem::absolute(res_path));
	if (!resource<T>::is_acquired(res_abs_path)) {
		resource<T>::acquire(res_abs_path);
	}
	return resource<T>::resources.at(res_abs_path);
}

template <typename T>
void resource<T>::acquire(const std::string &res_path) {
	std::filesystem::path res_abs_path(std::filesystem::absolute(res_path));
	if (!resource<T>::is_acquired(res_abs_path)) {
	        resources.insert(typename std::map<std::filesystem::path, T>::value_type((res_abs_path), T(res_abs_path)));
	}
}

template <typename T>
bool resource<T>::is_acquired(const std::string &res_path) {
	std::filesystem::path res_abs_path(std::filesystem::absolute(res_path));
	if (resources.find(res_abs_path) != resources.end()) 
		return true;
	return false;
}

template <typename T>
void resource<T>::erase(const std::string &res_path) {
	std::filesystem::path res_abs_path(std::filesystem::absolute(res_path));
	resources.erase(res_abs_path);
}


template <class T>
const T& get_resource(const std::string &res_path) {
	return resource<T>::get(res_path);
}


template <typename T>
class named_resource : public resource<T> {
private:
	named_resource() = delete;
	static std::map<std::string, std::filesystem::path> name_path_map;
public:

	static void acquire(const std::string &res_path, const std::string &name);
	static const T& get_by_name(const std::string &name);
	static bool is_acquired_by_name(const std::string &name);
	static void erase_by_name(const std::string &name);
	static void give_name(const std::string &res_path, const std::string name);
};

/* Initialize resources map */
template <class T>
std::map <std::string, std::filesystem::path> named_resource<T>::name_path_map;


template <typename T>
void named_resource<T>::acquire(const std::string &res_path, const std::string &name) {
	give_name(res_path, name);
	resource<T>::acquire(res_path);
}


template <typename T>
const T& named_resource<T>::get_by_name(const std::string &name) {
	return resource<T>::get(name_path_map.at(name));
}


template <typename T>
bool named_resource<T>::is_acquired_by_name(const std::string &name) {
	return resource<T>::is_acquired(name_path_map.at(name));
}

template <typename T>
void named_resource<T>::give_name(const std::string &res_path, const std::string name) {
		std::filesystem::path res_abs_path(std::filesystem::absolute(res_path));
		name_path_map[name] = res_path;
}


template <typename T>
void named_resource<T>::erase_by_name(const std::string &name) {
	resource<T>::erase(name_path_map.at(name));
}

template <class T>
const T& get_resource_by_name(const std::string &name) {
	return named_resource<T>::get_by_name(name);
}

}

#endif
