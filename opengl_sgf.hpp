#ifndef __OPENGLAPP_H__
#define __OPENGLAPP_H__

#include "keyboard.hpp"

namespace sgf {

class OpenGLGlutApp : public ControlsKeyboard {

private:
	inline static OpenGLGlutApp * m_running_instance = nullptr;
	static OpenGLGlutApp* get_runing_instance();
	/* Linkers with external API - reciving input from 
	 * keyboard, mouse ... from external sources and 
	 * handing it to our controlling mechanisms, this 
	 * linkers have same signitures as glut callbacks */
	static void glut_on_key_press(unsigned char key, int x, int y);
	static void glut_on_key_release(unsigned char key, int x, int y);
	static void glut_on_display();

protected: 
	void initialize(int *argc, char * argv[]);
	void run();
	void stop();
};


}

#endif 
