#include <iostream>
#include "../../sgf/include/sgf.hpp"

class Example :	public sgf::Entity,
		public sgf::UseMouse {
protected:

	void on_mouse_move(int x, int y, int x_old, int y_old) {
		std::cout << "Moved from (" << x_old << ", " << y_old << ") to (" << x << ", " << y << ")" << std::endl;
	}

	void on_mouse_button_press(sgf::MouseButton button, int x, int y) {
		std::cout << "Pressed " << (int) button << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}

	void on_mouse_button_release(sgf::MouseButton button, int x, int y) {
		std::cout << "Pressed " << (int) button << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}

	void on_mouse_scroll(sgf::MouseScrollDirection dir, int x, int y) {
		std::cout << "scroll dir: " << (int) dir << " from obj at position: (" << x << ", " <<  y << ")" << std::endl;
	}

	void update() {
		/* User have keyboard state object to use */
		if (this->mouse.is_pressed(sgf::MouseButton::LEFT)) {
			std::cout << "Left click is pressed (chk from Example obj)." << std::endl;
		}
	}

public:
sgf::App &operator >> (sgf::App& app) {
	app.connect_to_mouse(*this);
	return app;
}

};

void example_func(sgf::MouseScrollDirection dir, int x, int y) {
	std::cout << "scroll dir: " << (int) dir << " from example_func at position: (" << x << ", " <<  y << ")" << std::endl;
}

int main(int argc, char *argv[])
{
	sgf::App app = sgf::App();
	app.initialize(&argc, argv);

	Example e;
	/* Connecting object to app (with overloaded >> operator) */
	e >> app;

	/* Defining lambda to be passed and executed on_mouse_button_press */
	auto f = [] (sgf::MouseButton button, int x, int y) -> void {
		if (button == sgf::MouseButton::MIDDLE) {
			std::cout << "Pressed middle from lambda at (" << x << ", " << y << ")" << std::endl;
		}
	};

	/* Connecting lambda function to mouse (on button press) so it could use it */
	auto lambda_key_press_id = app.sig_mouse_button_pressed.connect(f);
	auto example_func_key_release_id = app.sig_mouse_scroll.connect(example_func);

	app.run();

	/* You can optionally manually disconnect it at any moment (if needed by logic)
	 * but it is not required since App will do it automatically if you don't */

	/* Disconnect by object reference */
	app.disconnect_from_mouse(e);

	/* Ordinary function must be disconnected by id */
	app.sig_mouse_button_pressed.disconnect(lambda_key_press_id);
	app.sig_mouse_button_released.disconnect(example_func_key_release_id);

	return 0;
};
