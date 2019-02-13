#include "opengl/opengl_glut_app.hpp"

namespace sgf {

class App : public sgf::framework::OpenGLGlutApp {
};

class Entity {
public:
virtual App& operator >> (App&) = 0;
};

}
