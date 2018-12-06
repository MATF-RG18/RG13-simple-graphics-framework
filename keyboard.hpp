#ifndef __SGF_KEYBOARD_H__
#define __SGF_KEYBOARD_H__
#include "signals.h"
#define NUM_OF_KEYS (127)

namespace sgf {

class ProcessesKeyboard;
class Keyboard;
class KeyboardController;

class ProcessesKeyboard {
protected:
	virtual void default_action(Keyboard *keyboard) = 0;
	virtual void on_key_press(Keyboard *keyboard, unsigned char key, int x, int y) = 0;
	virtual void on_key_release(Keyboard *keyboard, unsigned char key, int x, int y) = 0;

	friend class Keyboard;
	friend class KeyboardController;
};

class Keyboard {
	friend class KeyboardController;
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

class KeyboardController {
private: 
	Keyboard * keyboard;
	vdk::signal <void(Keyboard *keyboard)> m_default_action_sig;
public:
	KeyboardController(Keyboard *keyboard) : keyboard(keyboard) {};

	void connect(ProcessesKeyboard& kp);
	bool disconnect(ProcessesKeyboard& kp);
	void key_press(unsigned char key, int x, int y);
	void key_release(unsigned char key, int x, int y);
	void default_action();
};
}

#endif // __SGF_KEYBOARD_H__
