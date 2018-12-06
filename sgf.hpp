#ifndef __SGF_H__
#define __SGF_H__

#include <string>
#include <iostream>
#include "keyboard.hpp"

namespace sgf {

class App {
private:
	inline static App * m_running_instance = nullptr;

public:
	App() = default;
	void initialize(int *argc, char * argv[]);
	void run();

	static App* get_runing_instance();

	Keyboard keyboard;
	KeyboardController keyboard_controller{&keyboard};

protected:
	void deinitialize();
	void stop();
};

}

#endif // __SGF_H__
