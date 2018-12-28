#include <iostream>
#include "../../sgf/include/sgf.hpp"

class Object : public sgf::UseMouse {
protected:

	void on_mouse_update(sgf::Mouse *mouse) {
		std::cout << "default" << std::endl;
	}

	void on_mouse_move(sgf::Mouse *mouse, int x, int y, int x_old, int y_old) {
		std::cout << "Moved from (" << x_old << ", " << y_old << ") to (" << x << ", " << y << ")" << std::endl;
	}

	void on_mouse_button_press(sgf::Mouse *mouse, sgf::MouseButton button, int x, int y) {
		std::cout << "Pressed " << (int) button << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}

	void on_mouse_button_release(sgf::Mouse *mouse, sgf::MouseButton button, int x, int y) {
		std::cout << "Pressed " << (int) button << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}

	void on_mouse_scroll(sgf::Mouse *mouse, sgf::MouseScrollDirection dir, int x, int y) {
		std::cout << "scroll dir: " << (int) dir << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}


};

void example_func(sgf::Mouse * mouse, sgf::MouseScrollDirection dir, int x, int y) {
	std::cout << "scroll dir: " << (int) dir << " from example_func at position: (" << x << ", " <<  y << ")" << std::endl;
}

int main(int argc, char *argv[])
{
	sgf::App app = sgf::App();
	app.initialize(&argc, argv);   
	
	Object o;
	/* Connecting object to mouse so it could use it */
	app.connect_to_mouse(o);

	/* Defining lambda to be passed and executed on_mouse_button_press */
	auto f = [&] (sgf::Mouse * mouse, sgf::MouseButton button, int x, int y) -> void {
		if (button == sgf::MouseButton::MIDDLE) {
			std::cout << "Pressed middle from lambda at (" << x << ", " << y << ")" << std::endl;	
		}
	};

	/* Connecting function to mouse (on button press) so it could use it */
	auto lambda_key_press_id = app.connect_to_on_mouse_button_press(f);
	auto example_func_key_release_id = app.connect_to_on_mouse_scroll(example_func);
	
	app.run();

	/* You can optionally manually disconnect it at any moment (if needed by logic)
	 * but it is not required since App will do it automatically if you don't */

	/* Disconnect by object reference */
	app.disconnect_from_mouse(o);

	/* Functions must be disconnected by id */
	app.disconnect_from_on_mouse_button_press(lambda_key_press_id);
	app.disconnect_from_on_mouse_button_release(example_func_key_release_id);

};
