#include "vmc_app.hpp"
#include "simple_render_system.hpp"

// std
#include <cassert>
#include <stdexcept>
#include <array>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include<glm/gtc/constants.hpp>

namespace vmc {

	VmcApp::VmcApp()
	{
		loadGameObjects();
	}

	VmcApp::~VmcApp()
	{
	}

	void VmcApp::run()
	{
		SimpleRenderSystem simpleRenderSystem{ vmcDevice, vmcRenderer.getSwapChainRenderPass() };

		while (!vmcWindow.shouldClose())
		{
			glfwPollEvents();
			
			if (auto commandBuffer = vmcRenderer.beginFrame()) {
				vmcRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				vmcRenderer.endSwapChainRenderPass(commandBuffer);
				vmcRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(vmcDevice.device());
	}

	void VmcApp::loadGameObjects()
	{
		std::vector<VmcModel::Vertex> vertices{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };

		auto testModel = std::make_shared<VmcModel>(vmcDevice, vertices);

		auto triangle = VmcGameObject::createGameObject();
		triangle.model = testModel;
		triangle.color = { .1f, .8f, .1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = { 2.f, .5f };
		triangle.transform2d.rotation = 0.25f * glm::two_pi<float>();	// 90 degree rotation

		gameObjects.push_back(std::move(triangle));
	}


}