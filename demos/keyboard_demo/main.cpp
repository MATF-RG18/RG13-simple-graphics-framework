#include <iostream>
#include "../../sgf/include/sgf.hpp"

class Object : public sgf::ProcessesKeyboard {
protected:
	void default_key_action(sgf::Keyboard *keyboard) {
		std::cout << "default" << std::endl;
	}

	void on_key_press(sgf::Keyboard *keyboard, unsigned char key, int x, int y) {
		std::cout << "Pressed from obj: " << key << x << y << std::endl;
	}

	void on_key_release(sgf::Keyboard *keyboard, unsigned char key, int x, int y) {
		std::cout << "Released from obj: " << key << "(" << x << "," << y << ")" << std::endl;
	}
};

int main(int argc, char *argv[])
{
	sgf::App app = sgf::App();
	app.initialize(&argc, argv);   
	
	Object o;
	app.connect_to_keyboard(o);

	app.run();
};
