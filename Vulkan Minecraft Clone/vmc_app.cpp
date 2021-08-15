#include "vmc_app.hpp"
#include "simple_render_system.hpp"
#include "vmc_camera.hpp"
#include "keyboard_movement_controller.hpp"

// std
#include <cassert>
#include <stdexcept>
#include <array>
#include <chrono>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
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
        VmcCamera camera{};

        // Camera controller + camera state container (camera game object) --> WON'T BE RENDERED!
        // The camera's game object only manages the state (rotation + translation) of the camera.
        auto viewerObject = VmcGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!vmcWindow.shouldClose())
		{
			glfwPollEvents();

            // Time step (delta time)
            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;
            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

			// Update camera model (game object that contains camera
            cameraController.moveInPlaneXZ(vmcWindow.getGLFWwindow(), frameTime, viewerObject);
			// Update camera view matrix
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = vmcRenderer.getAspectRatio();

            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);

			// Render loop
			if (auto commandBuffer = vmcRenderer.beginFrame()) {
				vmcRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				vmcRenderer.endSwapChainRenderPass(commandBuffer);
				vmcRenderer.endFrame();
			}
		}
		vkDeviceWaitIdle(vmcDevice.device());
	}

 
	void VmcApp::loadGameObjects()
    {
		// Smooth vase 
		//std::shared_ptr<VmcModel> vmcModel = VmcModel::createModelFromFile(vmcDevice, "../Models/smooth_vase.obj");

  //      auto gameObj = VmcGameObject::createGameObject();
  //      gameObj.model = vmcModel;
  //      gameObj.transform.translation = { .0f, .0f, 2.5f };
  //      gameObj.transform.scale = { 1.f, 1.f, 1.f };
  //      gameObjects.push_back(std::move(gameObj));


		// Flat vase
		std::shared_ptr<VmcModel> vmcModel1 = VmcModel::createModelFromFile(vmcDevice, "../Models/flat_vase.obj");

		auto gameObj1 = VmcGameObject::createGameObject();
		gameObj1.model = vmcModel1;
		gameObj1.transform.translation = { .5f, .0f, 2.5f };
		gameObj1.transform.scale = { 1.f, 1.f, 1.f };
		gameObjects.push_back(std::move(gameObj1));

		// Chunk object
		auto chunkObj = VmcGameObject::createGameObject();
		std::shared_ptr<VmcModel> chunkModel = VmcModel::createChunkModelMesh(vmcDevice, chunkObj.chunk);
		chunkObj.model = chunkModel;
		chunkObj.transform.translation = { 2.0f, .0f, .0f };
		chunkObj.transform.scale = { .5f, .5f, .5f };
		gameObjects.push_back(std::move(chunkObj));
	}
}