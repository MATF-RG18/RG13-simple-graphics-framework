#include <iostream>
#include <GL/freeglut.h>
#include "../../include/opengl/opengl_glut_app.hpp"

namespace sgf::framework {

void OpenGLGlutApp::v_initialize(int *argc, char * argv[]) {
	glutInit(argc, argv);
	glutInitWindowSize(640, 480);
	glutCreateWindow("title");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_CONTINUE_EXECUTION);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	glutKeyboardFunc(OpenGLGlutApp::glut_on_key_press);
	glutKeyboardUpFunc(OpenGLGlutApp::glut_on_key_release);
	glutMouseFunc(glut_on_mouse_button);
	glutMotionFunc(glut_on_mouse_move);
	glutPassiveMotionFunc(glut_on_mouse_move);
	glutDisplayFunc(OpenGLGlutApp::glut_on_display);
	glClearColor(0,0,0,1);
}

void OpenGLGlutApp::glut_on_key_press(unsigned char c, int x, int y)
{
	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();
	active_app.keyboard_press_key(c, x, y);
}

void OpenGLGlutApp::glut_on_key_release(unsigned char c, int x, int y)
{
	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();
	active_app.keyboard_release_key(c, x, y);
}

void OpenGLGlutApp::glut_on_mouse_button(int button, int state, int x, int y)
{
	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();

	if (state == GLUT_DOWN) {

		switch (button) {

			case GLUT_LEFT_BUTTON : {
				active_app.mouse_press_button(sgf::MouseButton::LEFT, x, y);
			} break;

			case GLUT_RIGHT_BUTTON : {
				active_app.mouse_press_button(sgf::MouseButton::RIGHT, x, y);
			} break;

			case GLUT_MIDDLE_BUTTON : {
				active_app.mouse_press_button(sgf::MouseButton::MIDDLE, x, y);
			} break;

			case 3: {
				active_app.mouse_scroll(sgf::MouseScrollDirection::UP, x, y);
			} break;

		        case 4: {
				active_app.mouse_scroll(sgf::MouseScrollDirection::DOWN, x, y);
		          break;
		        }
	      }

	} else if (state == GLUT_UP) {

		switch (button) {

			case GLUT_LEFT_BUTTON : {
				active_app.mouse_release_button(sgf::MouseButton::LEFT, x, y);
			} break;

			case GLUT_RIGHT_BUTTON : {
				active_app.mouse_release_button(sgf::MouseButton::RIGHT, x, y);
			} break;

			case GLUT_MIDDLE_BUTTON : {
				active_app.mouse_release_button(sgf::MouseButton::MIDDLE, x, y);
			} break;

		}
	}

}

static int x_old = 0;
static int y_old = 0;
void OpenGLGlutApp::glut_on_mouse_move(int x, int y) {

	if (x == x_old && y == y_old)
		return;

	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();
	active_app.mouse_move(x, y, x_old, y_old);
	x_old = x;
	y_old = y;
}

void OpenGLGlutApp::glut_on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSwapBuffers();
}

OpenGLGlutApp* OpenGLGlutApp::get_runing_instance() {
	return OpenGLGlutApp::m_running_instance;
}

void OpenGLGlutApp::v_run() {
	OpenGLGlutApp::m_running_instance = this;
	glutMainLoop();
}


void OpenGLGlutApp::v_stop()
{
	glutLeaveMainLoop();
}

}

