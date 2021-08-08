#pragma once

#include "vmc_camera.hpp"
#include "vmc_pipeline.hpp"
#include "vmc_device.hpp"
#include "vmc_game_object.hpp"

// std 
#include <memory>
#include <vector>

namespace vmc {
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(VmcDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VmcGameObject> &gameObjects, const VmcCamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VmcDevice& vmcDevice;

		std::unique_ptr<VmcPipeline> vmcPipeline;
		VkPipelineLayout pipelineLayout;
	};
}
