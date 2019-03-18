#ifndef __SGF_FRAMEWORK_OPENGL_GLUT_APP_H__
#define __SGF_FRAMEWORK_OPENGL_GLUT_APP_H__

#include "../sgf/app.hpp"

namespace sgf::framework {

class OpenGLGlutApp : public App {
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
	static void glut_on_mouse_button(int button, int state, int x, int y);
	static void glut_on_mouse_move(int x, int y);
	//static void glut_on_mouse_scroll(int button, int dir, int x, int y);

protected: 
	void v_initialize(int *argc, char * argv[]);
	void v_run();
	void v_stop();
};


}

#endif
