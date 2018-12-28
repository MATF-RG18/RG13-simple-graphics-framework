#include "../../include/sgf/mouse.hpp"

namespace sgf {

std::tuple <unsigned, unsigned, unsigned, unsigned> Mouse::connect(UseMouse& mp) {
	return std::make_tuple(
		connect_to_on_mouse_move(mp),
		connect_to_on_mouse_button_press(mp),
		connect_to_on_mouse_button_release(mp),
		connect_to_on_mouse_scroll(mp)
	);
}

unsigned Mouse::connect_to_on_mouse_move(UseMouse& mp) {
	return m_mouse_move_sig.connect(&mp, &UseMouse::on_mouse_move);
}

unsigned Mouse::connect_to_on_mouse_button_press(UseMouse& mp) {
	return m_button_press_sig.connect(&mp, &UseMouse::on_mouse_button_press);
}

unsigned Mouse::connect_to_on_mouse_button_release(UseMouse& mp) {
	return m_button_release_sig.connect(&mp, &UseMouse::on_mouse_button_release);
}

unsigned Mouse::connect_to_on_mouse_scroll(UseMouse& mp) {
	return m_mouse_scroll_sig.connect(&mp, &UseMouse::on_mouse_scroll);
}

bool Mouse::disconnect_from_on_mouse_move(UseMouse& mp) {
	return m_mouse_move_sig.disconnect(&mp, &UseMouse::on_mouse_move);
}

bool Mouse::disconnect_from_on_mouse_button_press(UseMouse& mp) {
	return m_button_press_sig.disconnect(&mp, &UseMouse::on_mouse_button_press);
}

bool Mouse::disconnect_from_on_mouse_button_release(UseMouse& mp) {
	return m_button_release_sig.disconnect(&mp, &UseMouse::on_mouse_button_release);
}

bool Mouse::disconnect_from_on_mouse_scroll(UseMouse& mp) {
	return m_mouse_scroll_sig.disconnect(&mp, &UseMouse::on_mouse_scroll);
}

unsigned Mouse::connect_to_on_mouse_move(std::function<void(Mouse*, int x, int y, int x_old, int y_old)> f) {
	return m_mouse_move_sig.connect(f);
}

unsigned Mouse::connect_to_on_mouse_button_press(std::function<void(Mouse*, MouseButton button, int x, int y)> f) {
	return m_button_press_sig.connect(f);
}

unsigned Mouse::connect_to_on_mouse_button_release(std::function<void(Mouse*, MouseButton button, int x, int y)> f) {
	return m_button_release_sig.connect(f);
}

unsigned Mouse::connect_to_on_mouse_scroll(std::function<void(Mouse*, MouseScrollDirection dir, int x, int y)> f) {
	return m_mouse_scroll_sig.connect(f);
}

bool Mouse::disconnect_from_on_mouse_button_press(unsigned id) {
	return m_button_press_sig.disconnect(id);
}

bool Mouse::disconnect_from_on_mouse_button_release(unsigned id) {
	return m_button_release_sig.disconnect(id);
}

bool Mouse::disconnect_from_on_mouse_move(unsigned id) {
	return m_mouse_move_sig.disconnect(id);
}

bool Mouse::disconnect_from_on_mouse_scroll(unsigned id) {
	return m_mouse_scroll_sig.disconnect(id);
}

std::tuple <bool, bool, bool, bool> Mouse::disconnect(UseMouse& mp) {
	return std::make_tuple(disconnect_from_on_mouse_move(mp), disconnect_from_on_mouse_button_press(mp),
	disconnect_from_on_mouse_button_release(mp), disconnect_from_on_mouse_scroll(mp));
}

void Mouse::move(int x, int y, int x_old, int y_old) {
	m_mouse_move_sig.emit(this, x, y, x_old, y_old);
}

void Mouse::button_press(MouseButton button, int x, int y) {
	pressed_buttons[(int)button] = true;
	m_button_press_sig.emit(this, button, x, y);
}

void Mouse::button_release(MouseButton button, int x, int y) {
	pressed_buttons[(int)button] = false;
	m_button_release_sig.emit(this, button, x, y);
}

void Mouse::scroll(MouseScrollDirection dir, int x, int y) {
	m_mouse_scroll_sig.emit(this, dir, x, y);
}

bool Mouse::is_pressed(MouseButton button) {
	return pressed_buttons[(int)button];
}

MouseConnectRetVal framework::ControlsMouse::connect_to_mouse(UseMouse& mp) {
	auto up_id = connect_to_on_mouse_update(mp);
	auto k_ids = mouse.connect(mp); 
	return MouseConnectRetVal{up_id, std::get<0>(k_ids), std::get<1>(k_ids), std::get<2>(k_ids), std::get<3>(k_ids)};
}

MouseDisconnectRetVal framework::ControlsMouse::disconnect_from_mouse(UseMouse& mp) { 
	 auto up_id = disconnect_from_on_mouse_update(mp);
	 auto k_ids = mouse.disconnect(mp);
	 return MouseDisconnectRetVal{up_id, std::get<0>(k_ids), std::get<1>(k_ids), std::get<2>(k_ids), std::get<3>(k_ids)};
}

unsigned framework::ControlsMouse::connect_to_on_mouse_update(UseMouse& mp) {
	return m_update_mouse_sig.connect(&mp, &UseMouse::on_mouse_update); 
}

bool framework::ControlsMouse::disconnect_from_on_mouse_update(UseMouse& mp) {
	return m_update_mouse_sig.disconnect(&mp, &UseMouse::on_mouse_update);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_button_press(UseMouse& mp) {
	return mouse.connect_to_on_mouse_button_press(mp);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_button_press(UseMouse& mp) {
	return mouse.disconnect_from_on_mouse_button_press(mp);	
}

unsigned framework::ControlsMouse::connect_to_on_mouse_button_release(UseMouse& mp) {
	return mouse.connect_to_on_mouse_button_release(mp);
}

bool framework::ControlsMouse::disconnect_from_on_mouse_button_release(UseMouse& mp) {
	return mouse.disconnect_from_on_mouse_button_release(mp);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_update(std::function<void(Mouse*)> f) {
	return m_update_mouse_sig.connect(f);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_update(unsigned id) {
	return m_update_mouse_sig.disconnect(id);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_button_press(std::function<void(Mouse*, MouseButton button, int x, int y)> f) {
	return mouse.connect_to_on_mouse_button_press(f);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_button_press(unsigned id) {
	return mouse.disconnect_from_on_mouse_button_press(id);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_button_release(std::function<void(Mouse*, MouseButton button, int x, int y)> f) {
	return mouse.connect_to_on_mouse_button_release(f);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_button_release(unsigned id) {
	return mouse.disconnect_from_on_mouse_button_release(id);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_move(std::function<void(Mouse* mouse, int x, int y, int x_old, int y_old)> f) { 
	return mouse.connect_to_on_mouse_move(f);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_move(unsigned id) {
	return mouse.disconnect_from_on_mouse_move(id);
}

unsigned framework::ControlsMouse::connect_to_on_mouse_scroll(std::function<void(Mouse* mouse, MouseScrollDirection dir, int x, int y)> f) { 
	return mouse.connect_to_on_mouse_scroll(f);
}
bool framework::ControlsMouse::disconnect_from_on_mouse_scroll(unsigned id) {
	return mouse.disconnect_from_on_mouse_scroll(id);
}


void framework::ControlsMouse::mouse_move(int x, int y, int x_old, int y_old) { mouse.move(x, y, x_old, y_old); }
void framework::ControlsMouse::mouse_button_press(MouseButton button, int x, int y) { mouse.button_press(button, x, y); }
void framework::ControlsMouse::mouse_button_release(MouseButton button, int x, int y) { mouse.button_release(button, x, y); }
void framework::ControlsMouse::mouse_scroll(MouseScrollDirection dir, int x, int y) { mouse.scroll(dir, x, y); }
void framework::ControlsMouse::invoke_mouse_update() { m_update_mouse_sig.emit(&mouse); } 

}
