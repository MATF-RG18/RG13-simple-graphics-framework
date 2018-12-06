#include "sgf.hpp"

int main(int argc, char *argv[])
{
	sgf::App app = sgf::App();
	app.initialize(&argc, argv);   
	app.run();
};
