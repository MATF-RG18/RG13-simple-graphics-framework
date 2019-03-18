#include "../../../include/dependencies/abl/mouse.hpp"

namespace abl {

Mouse control::ControlsMouse::mouse;
const Mouse& UseMouse::mouse = control::ControlsMouse::get_mouse_instance();

const Mouse& control::ControlsMouse::get_mouse_instance() {
	return mouse;
}

void Mouse::move(int x, int y) {
	m_position = std::make_tuple(x, y);
}

void Mouse::press_button(MouseButton button) {
	m_pressed_buttons[button] = true;
}

void Mouse::release_button(MouseButton button) {
	m_pressed_buttons[button] = false;
}

bool Mouse::is_pressed(MouseButton button) const {
	if (m_pressed_buttons.find(button) != m_pressed_buttons.end())
		return m_pressed_buttons.at(button);
	return false;
}

const std::tuple<int, int>& Mouse::position() const {
	return m_position;
}

void control::ControlsMouse::mouse_move(int x, int y, int x_old, int y_old) {
	mouse.move(x, y);
	sig_mouse_moved.emit(x, y, x_old, y_old);
}

void control::ControlsMouse::mouse_press_button(MouseButton button, int x, int y) {
       	mouse.press_button(button);
	sig_mouse_button_pressed.emit(button, x, y);
}

void control::ControlsMouse::mouse_release_button(MouseButton button, int x, int y) {
	mouse.release_button(button);
	sig_mouse_button_released.emit(button, x, y);
}
void control::ControlsMouse::mouse_scroll(MouseScrollDirection dir, int x, int y) {
	sig_mouse_scroll.emit(dir, x, y);
}

MouseConnectRetVal control::ControlsMouse::connect_to_mouse(UseMouse& um) {
	unsigned move_id = sig_mouse_moved.connect(&um, &UseMouse::on_mouse_move);
	unsigned button_press_id = sig_mouse_button_pressed.connect(&um, &UseMouse::on_mouse_button_press);
	unsigned button_release_id = sig_mouse_button_released.connect(&um, &UseMouse::on_mouse_button_release);
	unsigned scroll_id = sig_mouse_scroll.connect(&um, &UseMouse::on_mouse_scroll);

	return {move_id, button_press_id, button_release_id, scroll_id};
}

MouseDisconnectRetVal control::ControlsMouse::disconnect_from_mouse(UseMouse& um) {
	bool move_chk = sig_mouse_moved.disconnect(&um, &UseMouse::on_mouse_move);
	bool button_press_chk = sig_mouse_button_pressed.disconnect(&um, &UseMouse::on_mouse_button_press);
	bool button_release_chk = sig_mouse_button_released.disconnect(&um, &UseMouse::on_mouse_button_release);
	bool scroll_chk = sig_mouse_scroll.disconnect(&um, &UseMouse::on_mouse_scroll);

	return {move_chk, button_press_chk, button_release_chk, scroll_chk};
}

}
