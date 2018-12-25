#include "../../include/sgf/keyboard.hpp"

namespace sgf {

std::tuple <unsigned, unsigned> Keyboard::connect(UseKeyboard& kp) {
	return std::make_tuple(
		connect_to_on_key_press(kp),
		connect_to_on_key_release(kp)
	);
}

unsigned Keyboard::connect_to_on_key_press(UseKeyboard& kp) {
	return m_key_press_sig.connect(&kp, &UseKeyboard::on_key_press);
}

unsigned Keyboard::connect_to_on_key_release(UseKeyboard& kp) {
	return m_key_release_sig.connect(&kp, &UseKeyboard::on_key_release);
}

bool Keyboard::disconnect_from_on_key_press(UseKeyboard& kp) {
	return m_key_press_sig.disconnect(&kp, &UseKeyboard::on_key_press);
}

bool Keyboard::disconnect_from_on_key_release(UseKeyboard& kp) {
	return m_key_release_sig.disconnect(&kp, &UseKeyboard::on_key_release);
}

unsigned Keyboard::connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return m_key_press_sig.connect(f);
}

bool Keyboard::disconnect_from_on_key_press(unsigned id) {
	return m_key_press_sig.disconnect(id);
}

unsigned Keyboard::connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return m_key_release_sig.connect(f);
}
bool Keyboard::disconnect_from_on_key_release(unsigned id) {
	return m_key_release_sig.disconnect(id);
}


std::tuple <bool, bool> Keyboard::disconnect(UseKeyboard& kp) {
	return std::make_tuple(disconnect_from_on_key_press(kp),
	disconnect_from_on_key_release(kp));
}

void Keyboard::key_press(unsigned char key, int x, int y) {
	pressed_keys[(int)key] = true;
	m_key_press_sig.emit(this, key, x, y);
}

void Keyboard::key_release(unsigned char key, int x, int y) {
	pressed_keys[(int)key] = false;
	m_key_release_sig.emit(this, key, x, y);
}

bool Keyboard::isPressed(char key) {
	return pressed_keys[(int)key];
}

KeyboardConnectRetVal framework::ControlsKeyboard::connect_to_keyboard(UseKeyboard& kp) {
	auto up_id = connect_to_on_keyboard_update(kp);
	auto k_ids = keyboard.connect(kp); 
	return KeyboardConnectRetVal{up_id, std::get<0>(k_ids), std::get<1>(k_ids)};
}

KeyboardDisconnectRetVal framework::ControlsKeyboard::disconnect_from_keyboard(UseKeyboard& kp) { 
	 auto up_id = disconnect_from_on_keyboard_update(kp);
	 auto k_ids = keyboard.disconnect(kp);
	 return KeyboardDisconnectRetVal{up_id, std::get<0>(k_ids), std::get<1>(k_ids)};
}

unsigned framework::ControlsKeyboard::connect_to_on_keyboard_update(UseKeyboard& kp) {
	return m_update_keyboard_sig.connect(&kp, &UseKeyboard::on_keyboard_update); 
}

bool framework::ControlsKeyboard::disconnect_from_on_keyboard_update(UseKeyboard& kp) {
	return m_update_keyboard_sig.disconnect(&kp, &UseKeyboard::on_keyboard_update);
}

unsigned framework::ControlsKeyboard::connect_to_on_key_press(UseKeyboard& kp) {
	return keyboard.connect_to_on_key_press(kp);
}
bool framework::ControlsKeyboard::disconnect_from_on_key_press(UseKeyboard& kp) {
	return keyboard.disconnect_from_on_key_press(kp);	
}

unsigned framework::ControlsKeyboard::connect_to_on_key_release(UseKeyboard& kp) {
	return keyboard.connect_to_on_key_release(kp);
}

bool framework::ControlsKeyboard::disconnect_from_on_key_release(UseKeyboard& kp) {
	return keyboard.disconnect_from_on_key_release(kp);
}

unsigned framework::ControlsKeyboard::connect_to_on_keyboard_update(std::function<void(Keyboard*)> f) {
	return m_update_keyboard_sig.connect(f);
}
bool framework::ControlsKeyboard::disconnect_from_on_keyboard_update(unsigned id) {
	return m_update_keyboard_sig.disconnect(id);
}

unsigned framework::ControlsKeyboard::connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return keyboard.connect_to_on_key_press(f);
}
bool framework::ControlsKeyboard::disconnect_from_on_key_press(unsigned id) {
	return keyboard.disconnect_from_on_key_press(id);
}

unsigned framework::ControlsKeyboard::connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return keyboard.connect_to_on_key_release(f);
}
bool framework::ControlsKeyboard::disconnect_from_on_key_release(unsigned id) {
	return keyboard.disconnect_from_on_key_release(id);
}

void framework::ControlsKeyboard::key_press(unsigned char key, int x, int y) { return keyboard.key_press(key, x, y); }
void framework::ControlsKeyboard::key_release(unsigned char key, int x, int y) { return keyboard.key_release(key, x, y); }
void framework::ControlsKeyboard::invoke_keyboard_update() { m_update_keyboard_sig.emit(&keyboard); } 

}
