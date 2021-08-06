#pragma once
#include "vmc_swap_chain.hpp"
#include "vmc_pipeline.hpp"
#include "vmc_device.hpp"
#include "vmc_window.hpp"
#include "vmc_game_object.hpp"

// std 
#include <memory>

namespace vmc {
	class VmcApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		VmcApp();
		~VmcApp();

		VmcApp(const VmcApp&) = delete;
		VmcApp& operator=(const VmcApp&) = delete;

		void run();
	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void loadGameObjects();
		void recreateSwapchain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);


		VmcWindow vmcWindow{ WIDTH, HEIGHT, "Hello Duyên!" };
		VmcDevice vmcDevice{ vmcWindow };
		std::unique_ptr<VmcSwapChain> vmcSwapChain;

		std::unique_ptr<VmcPipeline> vmcPipeline;
		VkPipelineLayout pipelineLayout;

		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VmcGameObject> gameObjects;
	};
}

