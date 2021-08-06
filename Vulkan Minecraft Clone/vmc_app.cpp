#include "vmc_app.hpp"

// std
#include <cassert>
#include <stdexcept>
#include <array>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vmc {

	struct TestPushConstant {
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	VmcApp::VmcApp()
	{
		loadModels();
		createPipelineLayout();
		recreateSwapchain();
		createCommandBuffers();
	}

	VmcApp::~VmcApp()
	{
		vkDestroyPipelineLayout(vmcDevice.device(), pipelineLayout, nullptr);
	}

	void VmcApp::run()
	{
		while (!vmcWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(vmcDevice.device());
	}

	void VmcApp::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(TestPushConstant);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(vmcDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw new std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void VmcApp::createPipeline()
	{
		assert(vmcSwapChain != nullptr && "Swap chain should be created before pipeline creation!");
		assert(pipelineLayout != nullptr && "Pipeline layout should be created before pipeline creation!");

		PipelineConfigInfo pipelineConfig{};
		 VmcPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = vmcSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vmcPipeline = std::make_unique<VmcPipeline>(vmcDevice, "../Shaders/simple_shader.vert.spv", "../Shaders/simple_shader.frag.spv", pipelineConfig);
	}

	void VmcApp::recreateSwapchain()
	{
		auto extent = vmcWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			// Let the program pause and wait when at least 1 dimension is 0.
			extent = vmcWindow.getExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(vmcDevice.device());
		if (vmcSwapChain == nullptr) {
			vmcSwapChain = std::make_unique<VmcSwapChain>(vmcDevice, extent);
		}
		else {
			vmcSwapChain = std::make_unique<VmcSwapChain>(vmcDevice, extent, std::move(vmcSwapChain));
			if (vmcSwapChain->imageCount() != commandBuffers.size()){
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		// Pipeline creation is dependant on the swapchain!
		createPipeline();
	}

	void VmcApp::createCommandBuffers()
	{
		commandBuffers.resize(vmcSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = vmcDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(vmcDevice.device(), &allocInfo, commandBuffers.data()) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void VmcApp::freeCommandBuffers()
	{
		vkFreeCommandBuffers(vmcDevice.device(), vmcDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void VmcApp::recordCommandBuffer(int imageIndex)
	{
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vmcSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vmcSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vmcSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vmcSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vmcSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vmcSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		vmcPipeline->bind(commandBuffers[imageIndex]);
		testModel->bind(commandBuffers[imageIndex]);
		for (int j = 0; j < 4; j++) {
			TestPushConstant push{};
			push.offset = { -0.5f + 0.01 * frame * 0.02f, -0.4f + j * 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * j };

			vkCmdPushConstants(
				commandBuffers[imageIndex],
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(TestPushConstant),
				&push);
			testModel->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}

	void VmcApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = vmcSwapChain->acquireNextImage(&imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = vmcSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vmcWindow.wasWindowResized()){
			vmcWindow.resetWindowResizedFlag();
			recreateSwapchain();
			return;
		}
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void VmcApp::loadModels()
	{
		std::vector<VmcModel::Vertex> vertices{
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}} };

		testModel = std::make_unique<VmcModel>(vmcDevice, vertices);
	}


}