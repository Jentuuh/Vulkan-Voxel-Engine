#include "vmc_window.hpp"

// std
#include <stdexcept>

namespace vmc {
	VmcWindow::VmcWindow(int w, int h, std::string name) : width{ w }, height{ h }, windowName{name}
	{
		initWindow();
	}

	VmcWindow::~VmcWindow() 
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VmcWindow::initWindow() 
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}

	void VmcWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface!");
		}
	}

}