#include "vmc_app.hpp"
#include "Chunk.hpp"
// std
#include <stdlib.h>
#include <iostream>
#include <stdexcept>

int main()
{
	vmc::VmcApp app{};
	try 
	{
		vmc::Chunk testChunk{ 16 };
		//app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}