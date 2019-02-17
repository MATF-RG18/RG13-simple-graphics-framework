#ifndef __SGF_MOUSE_H__
#define __SGF_MOUSE_H__
#include <utility>
#include <functional>
#include "../dependencies/signals.h"

namespace sgf {

/* Small data containing structures */
enum class MouseButton {NONE = 0, LEFT = 1, RIGHT = 2, MIDDLE = 3};
enum class MouseScrollDirection { DOWN = 0, UP = 1, NONE = 2 };

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
	friend class framework::ControlsMouse;
protected:
	virtual void on_mouse_update(const Mouse &mouse) = 0;
	virtual void on_mouse_move(const Mouse &mouse, int x, int y, int x_old, int y_old) = 0;
	virtual void on_mouse_button_press(const Mouse &mouse, MouseButton button, int x, int y) = 0;
	virtual void on_mouse_button_release(const Mouse &mouse, MouseButton button, int x, int y) = 0;
	virtual void on_mouse_scroll(const Mouse &mouse, MouseScrollDirection dir, int x, int y) = 0;
};

class Mouse {
	friend class sgf::framework::ControlsMouse;
private:
	std::unordered_map<MouseButton, bool> m_pressed_buttons;
	std::tuple<int, int> m_position = std::make_tuple(0, 0);

protected:
	void move(int x, int y);
	void press_button(MouseButton button);
	void release_button(MouseButton button);

public:
	bool is_pressed(MouseButton button);
	std::tuple<int, int> position();
};

namespace framework {

class ControlsMouse {
private:
	Mouse m_mouse;

protected:
	void mouse_move(int x, int y, int x_old, int y_old);
	void mouse_press_button(MouseButton button, int x, int y);
	void mouse_release_button(MouseButton button, int x, int y);
	void mouse_scroll(MouseScrollDirection dir, int x, int y);
	void mouse_invoke_update();

public:
	vdk::signal <void(const Mouse &mouse)> sig_mouse_update;
	vdk::signal <void(const Mouse &mouse, int x, int y, int x_old, int y_old)> sig_mouse_moved;
	vdk::signal <void(const Mouse &mouse, MouseButton button, int x, int y)> sig_mouse_button_pressed;
	vdk::signal <void(const Mouse &mouse, MouseButton button, int x, int y)> sig_mouse_button_released;
	vdk::signal <void(const Mouse &mouse, MouseScrollDirection dir, int x, int y)> sig_mouse_scroll;

	ControlsMouse() = default;
	MouseConnectRetVal connect_to_mouse(UseMouse& um);
	MouseDisconnectRetVal disconnect_from_mouse(UseMouse& um);
};

}

}

#endif // __SGF_MOUSE_H__
