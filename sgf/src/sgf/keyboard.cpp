#include "../../include/sgf/keyboard.hpp"

namespace sgf {

/* --------- Keyboard --------- */
void Keyboard::press_key(unsigned char key) {
	m_pressed_keys[key] = true;
}

void Keyboard::release_key(unsigned char key) {
	m_pressed_keys[key] = false;
}

bool Keyboard::is_pressed(char key) {
	if (m_pressed_keys.find(key) != m_pressed_keys.end())
		return m_pressed_keys[key];
	return false;
}

/* --------- ControlsKeyboard --------- */
void framework::ControlsKeyboard::keyboard_press_key(unsigned char key, int x, int y) {
	m_keyboard.press_key(key);
	sig_keyboard_key_pressed.emit(m_keyboard, key, x, y);
}

void framework::ControlsKeyboard::keyboard_release_key(unsigned char key, int x, int y) {
	m_keyboard.release_key(key);
	sig_keyboard_key_released.emit(m_keyboard, key, x, y);
}

void framework::ControlsKeyboard::keyboard_invoke_update() {
	sig_keyboard_update.emit(m_keyboard);
}

KeyboardConnectRetVal framework::ControlsKeyboard::connect_to_keyboard(UseKeyboard& uk) {
	unsigned key_press_id = sig_keyboard_key_pressed.connect(&uk, &UseKeyboard::on_keyboard_key_press);
	unsigned key_release_id = sig_keyboard_key_released.connect(&uk, &UseKeyboard::on_keyboard_key_release);
	unsigned update_keyboard_id = sig_keyboard_update.connect(&uk, &UseKeyboard::on_keyboard_update);

	return {key_press_id, key_release_id, update_keyboard_id};
}

KeyboardDisconnectRetVal framework::ControlsKeyboard::disconnect_from_keyboard(UseKeyboard& uk) {
	bool key_press_chk = sig_keyboard_key_pressed.disconnect(&uk, &UseKeyboard::on_keyboard_key_press);
	bool key_release_chk = sig_keyboard_key_released.disconnect(&uk, &UseKeyboard::on_keyboard_key_release);
	bool update_keyboard_chk = sig_keyboard_update.disconnect(&uk, &UseKeyboard::on_keyboard_update);

	return {key_press_chk, key_release_chk, update_keyboard_chk};
}

}
