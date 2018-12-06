#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "sgf.hpp"

namespace sgf {

void App::initialize(int *argc, char * argv[])
{
	OpenGLGlutApp::initialize(argc, argv);
}

void App::run()
{
	OpenGLGlutApp::run();
}

void App::stop() 
{
	OpenGLGlutApp::stop();
}

}
