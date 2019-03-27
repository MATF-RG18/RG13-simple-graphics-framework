#ifndef __SGF_APP_H__
#define __SGF_APP_H__

/* This file is fine processing to prepare *
 * frameworks outside look for end user    */

#include "opengl/opengl_glut_app.hpp"
#include "dependencies/abl/keyboard.hpp"
#include "dependencies/abl/mouse.hpp"
#include "dependencies/abl/resource.hpp"

namespace sgf {

/* Namespace renaming: for example vdk to sgf ... */
using namespace vdk; // signal will look sgf::signal
using namespace abl;
namespace framework {
	using namespace control;
}

class App : public sgf::framework::OpenGLGlutApp {
};

class Entity {
public:
virtual App& operator >> (App&) = 0;
};

}

#endif //__SGF_APP_H__
