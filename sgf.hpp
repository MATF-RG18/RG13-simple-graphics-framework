#ifndef __SGF_H__
#define __SGF_H__

#include <string>
#include <iostream>
#include "opengl_sgf.hpp"

namespace sgf {

class App : public OpenGLGlutApp
{
private:
public:
	App() {} 
	void initialize(int *argc, char * argv[]);
	void run();

protected:
	void deinitialize();
	void stop();
};

}

#endif // __SGF_H__
