#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "opengl/opengl_glut_app.hpp"

namespace sgf::framework {

void OpenGLGlutApp::v_initialize(int *argc, char * argv[]) {
	glutInit(argc, argv);
	glutInitWindowSize(640, 480);
	glutCreateWindow("title");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutKeyboardFunc(OpenGLGlutApp::glut_on_key_press);
	glutKeyboardUpFunc(OpenGLGlutApp::glut_on_key_release);
	glutDisplayFunc(OpenGLGlutApp::glut_on_display);
	glClearColor(0,0,0,1);
}

void OpenGLGlutApp::glut_on_key_press(unsigned char c, int x, int y)
{
	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();
	active_app.key_press(c, x, y);
}

void OpenGLGlutApp::glut_on_key_release(unsigned char c, int x, int y)
{
	OpenGLGlutApp & active_app = *OpenGLGlutApp::get_runing_instance();
	active_app.key_release(c, x, y);
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
}

}

