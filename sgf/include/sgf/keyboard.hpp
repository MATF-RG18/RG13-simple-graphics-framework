#ifndef __SGF_KEYBOARD_H__
#define __SGF_KEYBOARD_H__
#include <utility>
#include <functional>
#include "../dependencies/signals.h"
#define NUM_OF_KEYS (127)

namespace sgf {

struct KeyboardConnectRetVal {
	unsigned on_keyboard_update;
	unsigned on_key_press;
	unsigned on_key_release;
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
	friend class Keyboard;
	friend class framework::ControlsKeyboard;
protected:
	virtual void on_keyboard_update(Keyboard *keyboard) = 0;
	virtual void on_key_press(Keyboard *keyboard, unsigned char key, int x, int y) = 0;
	virtual void on_key_release(Keyboard *keyboard, unsigned char key, int x, int y) = 0;
	};

class Keyboard {
	friend class sgf::framework::ControlsKeyboard;
private:
	bool pressed_keys[NUM_OF_KEYS];
	vdk::signal <void(Keyboard *keyboard, unsigned char key, int x, int y)> m_key_press_sig;
	vdk::signal <void(Keyboard *keyboard, unsigned char key, int x, int y)> m_key_release_sig;

protected:
	void key_press(unsigned char key, int x, int y);
	void key_release(unsigned char key, int x, int y);

	std::tuple<unsigned, unsigned> connect(UseKeyboard& kp);
	std::tuple<bool, bool> disconnect(UseKeyboard& kp);
	unsigned connect_to_on_key_press(UseKeyboard& kp);
	bool disconnect_from_on_key_press(UseKeyboard& kp);
	unsigned connect_to_on_key_release(UseKeyboard& kp);
	bool disconnect_from_on_key_release(UseKeyboard& kp);

	unsigned connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_press(unsigned id);

	unsigned connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_release(unsigned id);

public:
	bool is_pressed(char key);

};

namespace framework { 

class ControlsKeyboard {
private:
	Keyboard keyboard;
	vdk::signal <void(Keyboard *keyboard)> m_update_keyboard_sig;

protected:
	void key_press(unsigned char key, int x, int y);
	void key_release(unsigned char key, int x, int y);
	void invoke_keyboard_update();

public:
	ControlsKeyboard() = default;
	KeyboardConnectRetVal connect_to_keyboard(UseKeyboard& kp);
	KeyboardDisconnectRetVal disconnect_from_keyboard(UseKeyboard& kp);

	unsigned connect_to_on_keyboard_update(UseKeyboard& kp);
	bool disconnect_from_on_keyboard_update(UseKeyboard& kp);
	unsigned connect_to_on_key_press(UseKeyboard& kp);
	bool disconnect_from_on_key_press(UseKeyboard& kp);
	unsigned connect_to_on_key_release(UseKeyboard& kp);
	bool disconnect_from_on_key_release(UseKeyboard& kp);

	unsigned connect_to_on_keyboard_update(std::function<void(Keyboard*)> f);
	bool disconnect_from_on_keyboard_update(unsigned id);

	unsigned connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_press(unsigned id);

	unsigned connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_release(unsigned id);
};
}

}

#endif // __SGF_KEYBOARD_H__
