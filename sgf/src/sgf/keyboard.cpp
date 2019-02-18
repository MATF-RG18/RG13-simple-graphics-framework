#include "../../include/sgf/keyboard.hpp"

namespace sgf {

Keyboard framework::ControlsKeyboard::keyboard;
const Keyboard& UseKeyboard::keyboard = framework::ControlsKeyboard::get_keyboard_instance();

/* --------- Keyboard --------- */
void Keyboard::press_key(unsigned char key) {
	m_pressed_keys[key] = true;
}

void Keyboard::release_key(unsigned char key) {
	m_pressed_keys[key] = false;
}

bool Keyboard::is_pressed(char key) const {
	if (m_pressed_keys.find(key) != m_pressed_keys.end())
		return m_pressed_keys.at(key);
	return false;
}

/* --------- ControlsKeyboard --------- */
const Keyboard& framework::ControlsKeyboard::get_keyboard_instance() {
	return keyboard;
}

void framework::ControlsKeyboard::keyboard_press_key(unsigned char key, int x, int y) {
	keyboard.press_key(key);
	sig_keyboard_key_pressed.emit(key, x, y);
}

void framework::ControlsKeyboard::keyboard_release_key(unsigned char key, int x, int y) {
	keyboard.release_key(key);
	sig_keyboard_key_released.emit(key, x, y);
}

KeyboardConnectRetVal framework::ControlsKeyboard::connect_to_keyboard(UseKeyboard& uk) {
	unsigned key_press_id = sig_keyboard_key_pressed.connect(&uk, &UseKeyboard::on_keyboard_key_press);
	unsigned key_release_id = sig_keyboard_key_released.connect(&uk, &UseKeyboard::on_keyboard_key_release);

	return {key_press_id, key_release_id};
}

KeyboardDisconnectRetVal framework::ControlsKeyboard::disconnect_from_keyboard(UseKeyboard& uk) {
	bool key_press_chk = sig_keyboard_key_pressed.disconnect(&uk, &UseKeyboard::on_keyboard_key_press);
	bool key_release_chk = sig_keyboard_key_released.disconnect(&uk, &UseKeyboard::on_keyboard_key_release);

	return {key_press_chk, key_release_chk};
}

}
