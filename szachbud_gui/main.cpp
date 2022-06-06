
#include "Config.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	Engine e{ "board.png" };


	e.MainLoop();

	std::cout << "Hello World!\n";

	return 0;
}
