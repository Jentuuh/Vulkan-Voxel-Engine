#include "vmc_app.hpp"

namespace vmc {

	void VmcApp::run() 
	{
		while (!vmcWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}