#ifndef __SGF_KEYBOARD_H__
#define __SGF_KEYBOARD_H__
#include <unordered_map>

#include "../dependencies/signals.h"

namespace sgf {

/* Small data containing structures */
/* Return values of connect/disconnect */
struct KeyboardConnectRetVal {
	unsigned on_keyboard_update_id;
	unsigned on_key_press_id;
	unsigned on_key_release_id;
};

struct KeyboardDisconnectRetVal {
	bool is_on_keyboard_update_disconnected;
	bool is_on_key_press_disconnected;
	bool is_on_key_release_disconnected;
};


class UseKeyboard;
class Keyboard;
namespace framework {class ControlsKeyboard;}

class UseKeyboard {
friend framework::ControlsKeyboard;
protected:
	virtual void on_keyboard_update(const Keyboard &keyboard) = 0;
	virtual void on_keyboard_key_press(const Keyboard &keyboard, unsigned char key, int x, int y) = 0;
	virtual void on_keyboard_key_release(const Keyboard &keyboard, unsigned char key, int x, int y) = 0;
};

class Keyboard {
friend class framework::ControlsKeyboard;
private:
	std::unordered_map<unsigned char, bool> m_pressed_keys;

protected:
	void press_key(unsigned char key);
	void release_key(unsigned char key);

public:
	bool is_pressed(char key);

};

namespace framework {

class ControlsKeyboard {
private:
	Keyboard m_keyboard;

protected:
	void keyboard_press_key(unsigned char key, int x, int y);
	void keyboard_release_key(unsigned char key, int x, int y);
	void keyboard_invoke_update();

public:
	vdk::signal<void(const Keyboard &keyboard)> sig_keyboard_update;
	vdk::signal<void(const Keyboard &keyboard, unsigned char key, int x, int y)> sig_keyboard_key_pressed;
	vdk::signal<void(const Keyboard &keyboard, unsigned char key, int x, int y)> sig_keyboard_key_released;

	KeyboardConnectRetVal connect_to_keyboard(UseKeyboard& uk);
	KeyboardDisconnectRetVal disconnect_from_keyboard(UseKeyboard& uk);
};
}

}

#endif // __SGF_KEYBOARD_H__
