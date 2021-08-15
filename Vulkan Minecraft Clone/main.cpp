#include "vmc_app.hpp"
// std
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

int main()
{
	vmc::VmcApp app{};
	try 
	{
		//vmc::ChunkComponent testchunk{ 16 };
		//testchunk.visibleBlockFacesTest();
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}