#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
namespace vmc {
	class VmcWindow
	{
	public:
		VmcWindow(int w, int h, std::string name);
		~VmcWindow();

		VmcWindow(const VmcWindow&) = delete;
		VmcWindow& operator=(const VmcWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	private:
		static void framebufferResizeCallback(GLFWwindow *window, int width, int height);
		void initWindow();

		int width;
		int height;

		bool framebufferResized = false;

		std::string windowName;
		GLFWwindow* window;
	};

}
