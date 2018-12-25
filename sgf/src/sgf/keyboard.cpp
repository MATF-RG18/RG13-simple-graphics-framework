#include "../../include/sgf/keyboard.hpp"

namespace sgf {

void Keyboard::connect(UseKeyboard& kp) {
	connect_to_on_key_press(kp);
	connect_to_on_key_release(kp);
}

void Keyboard::connect_to_on_key_press(UseKeyboard& kp) {
	m_key_press_sig.connect(&kp, &UseKeyboard::on_key_press);
}

void Keyboard::connect_to_on_key_release(UseKeyboard& kp) {
	m_key_release_sig.connect(&kp, &UseKeyboard::on_key_release);
}

bool Keyboard::disconnect_from_on_key_press(UseKeyboard& kp) {
	return m_key_press_sig.disconnect(&kp, &UseKeyboard::on_key_press);
}

bool Keyboard::disconnect_from_on_key_release(UseKeyboard& kp) {
	return m_key_release_sig.disconnect(&kp, &UseKeyboard::on_key_release);
}

void Keyboard::connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	m_key_press_sig.connect(f);
}

bool Keyboard::disconnect_from_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return m_key_press_sig.disconnect(f);
}

void Keyboard::connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	m_key_release_sig.connect(f);
}
bool Keyboard::disconnect_from_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	return m_key_release_sig.disconnect(f);
}


bool Keyboard::disconnect(UseKeyboard& kp) {

	bool is_all_disconnected = true &&
	disconnect_from_on_key_press(kp) &&
	disconnect_from_on_key_release(kp);

	return is_all_disconnected;
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

void framework::ControlsKeyboard::connect_to_keyboard(UseKeyboard& kp) {
	connect_to_on_keyboard_update(kp);
	keyboard.connect(kp); 
}

bool framework::ControlsKeyboard::disconnect_from_keyboard(UseKeyboard& kp) { 
	return keyboard.disconnect(kp) && 
		disconnect_from_on_keyboard_update(kp);
}

void framework::ControlsKeyboard::connect_to_on_keyboard_update(UseKeyboard& kp) {
	m_update_keyboard_sig.connect(&kp, &UseKeyboard::on_keyboard_update); 
}

bool framework::ControlsKeyboard::disconnect_from_on_keyboard_update(UseKeyboard& kp) {
	return m_update_keyboard_sig.disconnect(&kp, &UseKeyboard::on_keyboard_update);
}

void framework::ControlsKeyboard::connect_to_on_key_press(UseKeyboard& kp) {
	keyboard.connect_to_on_key_press(kp);
}
bool framework::ControlsKeyboard::disconnect_from_on_key_press(UseKeyboard& kp) {
	return keyboard.disconnect_from_on_key_press(kp);	
}

void framework::ControlsKeyboard::connect_to_on_key_release(UseKeyboard& kp) {
	keyboard.connect_to_on_key_release(kp);
}

bool framework::ControlsKeyboard::disconnect_from_on_key_release(UseKeyboard& kp) {
	return keyboard.disconnect_from_on_key_release(kp);
}

void framework::ControlsKeyboard::connect_to_on_keyboard_update(std::function<void(Keyboard*)> f) {
	m_update_keyboard_sig.connect(f);
}
bool framework::ControlsKeyboard::disconnect_func_from_on_keyboard_update(std::function<void(Keyboard*)> f) {
	return m_update_keyboard_sig.disconnect(f);
}

void framework::ControlsKeyboard::connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	keyboard.connect_to_on_key_press(f);
}
void framework::ControlsKeyboard::disconnect_func_from_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	keyboard.disconnect_from_on_key_press(f);
}

void framework::ControlsKeyboard::connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	keyboard.connect_to_on_key_release(f);
}
void framework::ControlsKeyboard::disconnect_from_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f) {
	keyboard.disconnect_from_on_key_release(f);
}

void framework::ControlsKeyboard::key_press(unsigned char key, int x, int y) { return keyboard.key_press(key, x, y); }
void framework::ControlsKeyboard::key_release(unsigned char key, int x, int y) { return keyboard.key_release(key, x, y); }
void framework::ControlsKeyboard::invoke_keyboard_update() { m_update_keyboard_sig.emit(&keyboard); } 

}
