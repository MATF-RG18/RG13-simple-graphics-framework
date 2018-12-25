#ifndef __SGF_KEYBOARD_H__
#define __SGF_KEYBOARD_H__
#include <functional>
#include "../dependencies/signals.h"
#define NUM_OF_KEYS (127)

namespace sgf {

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

	void connect(UseKeyboard& kp);
	bool disconnect(UseKeyboard& kp);
	void connect_to_on_key_press(UseKeyboard& kp);
	bool disconnect_from_on_key_press(UseKeyboard& kp);
	void connect_to_on_key_release(UseKeyboard& kp);
	bool disconnect_from_on_key_release(UseKeyboard& kp);

	void connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);

	void connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	bool disconnect_from_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);

public:
	bool isPressed(char key);

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
	void connect_to_keyboard(UseKeyboard& kp);
	bool disconnect_from_keyboard(UseKeyboard& kp);

	void connect_to_on_keyboard_update(UseKeyboard& kp);
	bool disconnect_from_on_keyboard_update(UseKeyboard& kp);
	void connect_to_on_key_press(UseKeyboard& kp);
	bool disconnect_from_on_key_press(UseKeyboard& kp);
	void connect_to_on_key_release(UseKeyboard& kp);
	bool disconnect_from_on_key_release(UseKeyboard& kp);

	void connect_to_on_keyboard_update(std::function<void(Keyboard*)> f);
	bool disconnect_func_from_on_keyboard_update(std::function<void(Keyboard*)> f);

	void connect_to_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	void disconnect_func_from_on_key_press(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);

	void connect_to_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
	void disconnect_from_on_key_release(std::function<void(Keyboard*, unsigned char key, int x, int y)> f);
};
}

}

#endif // __SGF_KEYBOARD_H__
