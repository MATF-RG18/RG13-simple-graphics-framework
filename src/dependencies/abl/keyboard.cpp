#include "../../../include/dependencies/abl/keyboard.hpp"

namespace abl {

Keyboard control::ControlsKeyboard::keyboard;
const Keyboard& UseKeyboard::keyboard = control::ControlsKeyboard::get_keyboard_instance();

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
const Keyboard& control::ControlsKeyboard::get_keyboard_instance() {
	return keyboard;
}

void control::ControlsKeyboard::keyboard_press_key(unsigned char key, int x, int y) {
	keyboard.press_key(key);
	sig_keyboard_key_pressed.emit(key, x, y);
}

void control::ControlsKeyboard::keyboard_release_key(unsigned char key, int x, int y) {
	keyboard.release_key(key);
	sig_keyboard_key_released.emit(key, x, y);
}

KeyboardConnectRetVal control::ControlsKeyboard::connect_to_keyboard(UseKeyboard& uk) {
	unsigned key_press_id = sig_keyboard_key_pressed.connect(&uk, &UseKeyboard::on_keyboard_key_press);
	unsigned key_release_id = sig_keyboard_key_released.connect(&uk, &UseKeyboard::on_keyboard_key_release);

	return {key_press_id, key_release_id};
}

KeyboardDisconnectRetVal control::ControlsKeyboard::disconnect_from_keyboard(UseKeyboard& uk) {
	bool key_press_chk = sig_keyboard_key_pressed.disconnect(&uk, &UseKeyboard::on_keyboard_key_press);
	bool key_release_chk = sig_keyboard_key_released.disconnect(&uk, &UseKeyboard::on_keyboard_key_release);

	return {key_press_chk, key_release_chk};
}

}
