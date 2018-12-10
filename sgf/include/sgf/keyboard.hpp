#ifndef __SGF_KEYBOARD_H__
#define __SGF_KEYBOARD_H__
#include "../dependencies/signals.h"
#define NUM_OF_KEYS (127)

namespace sgf {

class ProcessesKeyboard;
class Keyboard;
namespace framework {class ControlsKeyboard;}

class ProcessesKeyboard {
	friend class Keyboard;
	friend class framework::ControlsKeyboard;
protected:
	virtual void default_key_action(Keyboard *keyboard) = 0;
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
	void connect(ProcessesKeyboard& kp);
	bool disconnect(ProcessesKeyboard& kp);
	void key_press(unsigned char key, int x, int y);
	void key_release(unsigned char key, int x, int y);
public:

};

namespace framework { 

class ControlsKeyboard {
private:
	Keyboard keyboard;
	vdk::signal <void(Keyboard *keyboard)> m_default_key_action_sig;

protected:
	void key_press(unsigned char key, int x, int y);
	void key_release(unsigned char key, int x, int y);
	void invoke_default_key_action();

public:
	ControlsKeyboard() = default;
	void connect_to_keyboard(ProcessesKeyboard& kp);
	bool disconnect_from_keyboard(ProcessesKeyboard& kp);
};
}

}

#endif // __SGF_KEYBOARD_H__
