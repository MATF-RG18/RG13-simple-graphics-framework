#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "sgf.hpp"

namespace sgf {

static void on_display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glutSwapBuffers();
}

static void on_key_press(unsigned char c, int x, int y)
{
	App & active_app = *App::get_runing_instance();
	active_app.keyboard_controller.key_press(c, x, y);
}

static void on_key_release(unsigned char c, int x, int y)
{
	App & active_app = *App::get_runing_instance();
	active_app.keyboard_controller.key_release(c, x, y);
}


void App::initialize(int *argc, char * argv[])
{

	glutInit(argc, argv);
	glutInitWindowSize(640, 480);
	glutCreateWindow("title");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

	glutKeyboardFunc(on_key_press);
	glutKeyboardUpFunc(on_key_release);
	glutDisplayFunc(on_display);
	glClearColor(0,0,0,1);

	class Object : public ProcessesKeyboard {
	protected:
		void default_action(Keyboard *keyboard) {
			std::cout << "default" << std::endl;
		}

		void on_key_press(Keyboard *keyboard, unsigned char key, int x, int y) {
			std::cout << "Pressed from obj: " << key << x << y << std::endl;
		}

		void on_key_release(Keyboard *keyboard, unsigned char key, int x, int y) {
			std::cout << "Released from obj: " << key << "(" << x << "," << y << ")" << std::endl;
		}
	};

	Object o;
	keyboard_controller.connect(o);
}

void App::run()
{
	App::m_running_instance = this;

	glutMainLoop();
}

void App::stop() 
{
	App::m_running_instance = nullptr;
}

App* App::get_runing_instance() {
	return App::m_running_instance;
}
}
