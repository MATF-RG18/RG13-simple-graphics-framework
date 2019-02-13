#include <iostream>
#include "../../sgf/include/sgf.hpp"

class Object : 	public sgf::Entity,
		public sgf::UseKeyboard {
protected:

	/* User should define default action regards to pressed keys - Required by UseKeyboard */
	void on_keyboard_update(sgf::Keyboard *keyboard) {
		std::cout << "default" << std::endl;
	}

	/* User should define on_key press/release action - Required by UseKeyboard */
	void on_key_press(sgf::Keyboard *keyboard, unsigned char key, int x, int y) {
		std::cout << "Pressed " << key << " from obj at position: (" << x << ", " <<  y << std::endl;
	}
	void on_key_release(sgf::Keyboard *keyboard, unsigned char key, int x, int y) {
		std::cout << "Released " << key << " from obj at position: (" << x << ", " << y << ")" << std::endl;
	}
public:
sgf::App& operator >> (sgf::App& app) {
	app.connect_to_keyboard(*this);
	return app;
}

};


void example_func(sgf::Keyboard * kb, unsigned char key, int x, int y) {
	std::cout << "Release from example function " << std::endl;
}

int main(int argc, char *argv[])
{
	sgf::App app = sgf::App();
	app.initialize(&argc, argv);   
	
	Object o;
	/* Connecting object to keyboard so it could use it */
	/* Insering object data to app, by overloaded >> operator */
	o >> app;

	/* Defining lambda to be passed and executed on_key_press */
	auto f = [&] (sgf::Keyboard * kb, unsigned char key, int x, int y) -> void {
		if (key == 27) {
			app.stop();
		}
	};

	/* Connecting function to keyboard (on key press) so it could use it */
	auto lambda_key_press_id = app.connect_to_on_key_press(f);
	auto example_func_key_release_id = app.connect_to_on_key_release(example_func);
	
	app.run();

	/* You can optionally manually disconnect it at any moment (if needed by logic)
	 * but it is not required since App will do it automatically if you don't */

	/* Disconnect by object reference */
	app.disconnect_from_keyboard(o);

	/* Functions must be disconnected by id */
	app.disconnect_from_on_key_press(lambda_key_press_id);
	app.disconnect_from_on_key_press(example_func_key_release_id);

};
