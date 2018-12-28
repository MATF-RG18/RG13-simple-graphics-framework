#ifndef __SGF_FRAMEWORK_APP_H__
#define __SGF_FRAMEWORK_APP_H__

#include "keyboard.hpp"
#include "mouse.hpp"

namespace sgf::framework {

class App : public ControlsKeyboard, 
	    public ControlsMouse 
{
private:
protected:
	virtual void v_initialize(int * argc, char * argv[]) = 0;
	virtual void v_run() = 0;
	virtual void v_stop() = 0;
public:
	App() = default;
	void initialize(int * argc, char * argv[]);
	void run();
	void stop();
};

}

#endif
