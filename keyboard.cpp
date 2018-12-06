#include "keyboard.hpp"

namespace sgf {

void Keyboard::connect(ProcessesKeyboard& kp) {
	m_key_press_sig.connect(&kp, &ProcessesKeyboard::on_key_press);
	m_key_release_sig.connect(&kp, &ProcessesKeyboard::on_key_release);
}

bool Keyboard::disconnect(ProcessesKeyboard& kp) {

	bool is_all_disconnected = true &&
	m_key_press_sig.disconnect(&kp, &ProcessesKeyboard::on_key_press) &&
	m_key_release_sig.disconnect(&kp, &ProcessesKeyboard::on_key_release);

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

void KeyboardController::connect(ProcessesKeyboard& kp) {
	m_default_action_sig.connect(&kp, &ProcessesKeyboard::default_action); 
	keyboard->connect(kp); 
}

bool KeyboardController::disconnect(ProcessesKeyboard& kp) { 
	return keyboard->disconnect(kp) && 
		m_default_action_sig.disconnect(&kp, &ProcessesKeyboard::default_action);
}

void KeyboardController::key_press(unsigned char key, int x, int y) { return keyboard->key_press(key, x, y); }
void KeyboardController::key_release(unsigned char key, int x, int y) { return keyboard->key_release(key, x, y); }
void KeyboardController::default_action() { m_default_action_sig.emit(keyboard); }

}
