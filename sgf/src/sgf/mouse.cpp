#include "../../include/sgf/mouse.hpp"

namespace sgf {

void Mouse::move(int x, int y) {
	m_position = std::make_tuple(x, y);
}

void Mouse::press_button(MouseButton button) {
	m_pressed_buttons[button] = true;
}

void Mouse::release_button(MouseButton button) {
	m_pressed_buttons[button] = false;
}

bool Mouse::is_pressed(MouseButton button) {
	if (m_pressed_buttons.find(button) != m_pressed_buttons.end())
		return m_pressed_buttons[button];
	return false;
}

std::tuple<int, int> Mouse::position() {
	return m_position;
}

void framework::ControlsMouse::mouse_move(int x, int y, int x_old, int y_old) {
	m_mouse.move(x, y);
	sig_mouse_moved.emit(m_mouse, x, y, x_old, y_old);
}

void framework::ControlsMouse::mouse_press_button(MouseButton button, int x, int y) {
       	m_mouse.press_button(button);
	sig_mouse_button_pressed.emit(m_mouse, button, x, y);
}

void framework::ControlsMouse::mouse_release_button(MouseButton button, int x, int y) {
	m_mouse.release_button(button);
	sig_mouse_button_released.emit(m_mouse, button, x, y);
}
void framework::ControlsMouse::mouse_scroll(MouseScrollDirection dir, int x, int y) {
	sig_mouse_scroll.emit(m_mouse, dir, x, y);
}

void framework::ControlsMouse::mouse_invoke_update() {
	sig_mouse_update.emit(m_mouse);
}

MouseConnectRetVal framework::ControlsMouse::connect_to_mouse(UseMouse& um) {
	unsigned update_id = sig_mouse_update.connect(&um, &UseMouse::on_mouse_update);
	unsigned move_id = sig_mouse_moved.connect(&um, &UseMouse::on_mouse_move);
	unsigned button_press_id = sig_mouse_button_pressed.connect(&um, &UseMouse::on_mouse_button_press);
	unsigned button_release_id = sig_mouse_button_released.connect(&um, &UseMouse::on_mouse_button_release);
	unsigned scroll_id = sig_mouse_scroll.connect(&um, &UseMouse::on_mouse_scroll);

	return {update_id, move_id, button_press_id, button_release_id, scroll_id};
}

MouseDisconnectRetVal framework::ControlsMouse::disconnect_from_mouse(UseMouse& um) {

	bool update_chk = sig_mouse_update.disconnect(&um, &UseMouse::on_mouse_update);
	bool move_chk = sig_mouse_moved.disconnect(&um, &UseMouse::on_mouse_move);
	bool button_press_chk = sig_mouse_button_pressed.disconnect(&um, &UseMouse::on_mouse_button_press);
	bool button_release_chk = sig_mouse_button_released.disconnect(&um, &UseMouse::on_mouse_button_release);
	bool scroll_chk = sig_mouse_scroll.disconnect(&um, &UseMouse::on_mouse_scroll);

	return {update_chk, move_chk, button_press_chk, button_release_chk, scroll_chk};
}

}
