#ifndef __SGF_MOUSE_H__
#define __SGF_MOUSE_H__
#include <utility>
#include <functional>
#include "../dependencies/signals.h"
#define NUM_OF_BUTTONS (4)

namespace sgf {

enum class MouseButton {NONE = 0, LEFT = 1, RIGHT = 2, MIDDLE = 3};
enum class MouseScrollDirection { DOWN = 0, UP = 1 };

struct MouseConnectRetVal {
	unsigned on_mouse_update;
	unsigned on_mouse_move;
	unsigned on_mouse_button_press;
	unsigned on_mouse_button_release;
	unsigned on_mouse_scroll;
};

struct MouseDisconnectRetVal {
	 bool is_on_mouse_update_disconnected;
	 bool is_on_mouse_move_disconnected;
	 bool is_on_mouse_button_press_disconnected;
	 bool is_on_mouse_button_release_disconnected;
	 bool is_on_mouse_scroll_disconnected;
	
};

class UseMouse;
class Mouse;
namespace framework {class ControlsMouse;}

class UseMouse {
	friend class Mouse;
	friend class framework::ControlsMouse;
protected:
	virtual void on_mouse_update(Mouse *mouse) = 0;
	virtual void on_mouse_move(Mouse *mouse, int x, int y, int x_old, int y_old) = 0;
	virtual void on_mouse_button_press(Mouse *mouse, MouseButton button, int x, int y) = 0;
	virtual void on_mouse_button_release(Mouse *mouse, MouseButton button, int x, int y) = 0;
	virtual void on_mouse_scroll(Mouse * mouse, MouseScrollDirection dir, int x, int y) = 0;
};

class Mouse {
	friend class sgf::framework::ControlsMouse;
private:
	bool pressed_buttons[NUM_OF_BUTTONS];
	vdk::signal <void(Mouse *mouse, int x, int y, int x_old, int y_old)> m_mouse_move_sig;
	vdk::signal <void(Mouse *mouse, MouseButton button, int x, int y)> m_button_press_sig;
	vdk::signal <void(Mouse *mouse, MouseButton button, int x, int y)> m_button_release_sig;
	vdk::signal <void(Mouse *mouse, MouseScrollDirection dir, int x, int y)> m_mouse_scroll_sig;

protected:
	void move(int x, int y, int x_old, int y_old);
	void button_press(MouseButton button, int x, int y);
	void button_release(MouseButton button, int x, int y);
	void scroll(MouseScrollDirection dir, int x, int y);

	std::tuple<unsigned, unsigned, unsigned, unsigned> connect(UseMouse& kp);
	std::tuple<bool, bool, bool, bool> disconnect(UseMouse& kp);
	unsigned connect_to_on_mouse_move(UseMouse& kp);
	bool disconnect_from_on_mouse_move(UseMouse& kp);
	unsigned connect_to_on_mouse_button_press(UseMouse& kp);
	bool disconnect_from_on_mouse_button_press(UseMouse& kp);
	unsigned connect_to_on_mouse_button_release(UseMouse& kp);
	bool disconnect_from_on_mouse_button_release(UseMouse& kp);
	unsigned connect_to_on_mouse_scroll(UseMouse& kp);
	bool disconnect_from_on_mouse_scroll(UseMouse& kp);

	unsigned connect_to_on_mouse_move(std::function<void(Mouse*, int x, int y, int x_old, int y_old)> f);
	bool disconnect_from_on_mouse_move(unsigned id);

	unsigned connect_to_on_mouse_button_press(std::function<void(Mouse*, MouseButton button, int x, int y)> f);
	bool disconnect_from_on_mouse_button_press(unsigned id);

	unsigned connect_to_on_mouse_button_release(std::function<void(Mouse*, MouseButton button, int x, int y)> f);
	bool disconnect_from_on_mouse_button_release(unsigned id);

	unsigned connect_to_on_mouse_scroll(std::function<void(Mouse*, MouseScrollDirection dir, int x, int y)> f);
	bool disconnect_from_on_mouse_scroll(unsigned id);


public:
	bool is_pressed(MouseButton button);

};

namespace framework { 

class ControlsMouse {
private:
	Mouse mouse;
	vdk::signal <void(Mouse *mouse)> m_update_mouse_sig;

protected:
	void mouse_move(int x, int y, int x_old, int y_old);
	void mouse_button_press(MouseButton button, int x, int y);
	void mouse_button_release(MouseButton button, int x, int y);
	void mouse_scroll(MouseScrollDirection dir, int x, int y);
	void invoke_mouse_update();

public:
	ControlsMouse() = default;
	MouseConnectRetVal connect_to_mouse(UseMouse& kp);
	MouseDisconnectRetVal disconnect_from_mouse(UseMouse& kp);

	unsigned connect_to_on_mouse_update(UseMouse& kp);
	bool disconnect_from_on_mouse_update(UseMouse& kp);
	unsigned connect_to_on_mouse_move(UseMouse& kp);
	bool disconnect_from_on_mouse_move(UseMouse& kp);
	unsigned connect_to_on_mouse_button_press(UseMouse& kp);
	bool disconnect_from_on_mouse_button_press(UseMouse& kp);
	unsigned connect_to_on_mouse_button_release(UseMouse& kp);
	bool disconnect_from_on_mouse_button_release(UseMouse& kp);
	unsigned connect_to_on_mouse_scroll(UseMouse& kp);
	bool disconnect_from_on_mouse_scroll(UseMouse& kp);


	unsigned connect_to_on_mouse_update(std::function<void(Mouse* mouse)> f);
	bool disconnect_from_on_mouse_update(unsigned id);

	unsigned connect_to_on_mouse_move(std::function<void(Mouse* mouse, int x, int y, int x_old, int y_old)> f);
	bool disconnect_from_on_mouse_move(unsigned id);

	unsigned connect_to_on_mouse_button_press(std::function<void(Mouse* mouse, MouseButton button, int x, int y)> f);
	bool disconnect_from_on_mouse_button_press(unsigned id);

	unsigned connect_to_on_mouse_button_release(std::function<void(Mouse* mouse, MouseButton button, int x, int y)> f);
	bool disconnect_from_on_mouse_button_release(unsigned id);

	unsigned connect_to_on_mouse_scroll(std::function<void(Mouse* mouse, MouseScrollDirection dir, int x, int y)> f);
	bool disconnect_from_on_mouse_scroll(unsigned id);

};
}

}

#endif // __SGF_MOUSE_H__
