#include "vmc_renderer.hpp"

// std
#include <cassert>
#include <stdexcept>
#include <array>

namespace vmc {

	VmcRenderer::VmcRenderer(VmcWindow & window, VmcDevice & device) : vmcWindow{window}, vmcDevice{device}
	{
		recreateSwapchain();
		createCommandBuffers();
	}

	VmcRenderer::~VmcRenderer()
	{
		freeCommandBuffers();
	}


	void VmcRenderer::recreateSwapchain()
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
			std::shared_ptr<VmcSwapChain> oldSwapChain = std::move(vmcSwapChain);
			vmcSwapChain = std::make_unique<VmcSwapChain>(vmcDevice, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormats(*vmcSwapChain.get())) {
				throw std::runtime_error("Swap chain image (or depth image) format has changed!");
			}
		}
	}

	void VmcRenderer::createCommandBuffers()
	{
		commandBuffers.resize(VmcSwapChain::MAX_FRAMES_IN_FLIGHT);

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

	void VmcRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(vmcDevice.device(), vmcDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}


	VkCommandBuffer VmcRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Cannot call beginFrame when frame has already started!");

		auto result = vmcSwapChain->acquireNextImage(&currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			recreateSwapchain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		isFrameStarted = true;
		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void VmcRenderer::endFrame()
	{
		assert(isFrameStarted && "Cannot end the frame when there is no current frame in progress!");
		auto commandBuffer = getCurrentCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}

		auto result = vmcSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || vmcWindow.wasWindowResized()) {
			vmcWindow.resetWindowResizedFlag();
			recreateSwapchain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image!");
		}
		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % VmcSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void VmcRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Cannot begin the render pass when there is no current frame in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cannot begin render pass on command buffer from a different frame!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vmcSwapChain->getRenderPass();
		renderPassInfo.framebuffer = vmcSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = vmcSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(vmcSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(vmcSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, vmcSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void VmcRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Cannot end the render pass when there is no current frame in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Cannot end render pass on command buffer from a different frame!");
		vkCmdEndRenderPass(commandBuffer);

	}
}