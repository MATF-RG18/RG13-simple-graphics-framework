#include "../../include/sgf/app.hpp"

namespace sgf::framework {

void App::run() {
	v_run();
}

void App::stop() {
	v_stop();
}

void App::initialize(int * argc, char * argv[]) {
	v_initialize(argc, argv);
}

}
