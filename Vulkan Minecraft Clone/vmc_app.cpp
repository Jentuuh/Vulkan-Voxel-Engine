#include "vmc_app.hpp"

// std
#include <stdexcept>

namespace vmc {
	VmcApp::VmcApp()
	{
		createPipelineLayout();
		createPipeline();
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
		}
	}

	void VmcApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(vmcDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw new std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void VmcApp::createPipeline()
	{
		auto pipelineConfig = VmcPipeline::defaultPipelineConfigInfo(vmcSwapChain.width(), vmcSwapChain.height());
		pipelineConfig.renderPass = vmcSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vmcPipeline = std::make_unique<VmcPipeline>(vmcDevice, "../Shaders/simple_shader.vert.spv", "../Shaders/simple_shader.frag.spv", pipelineConfig);
	}


	void VmcApp::createCommandBuffers()
	{

	}

	void VmcApp::drawFrame()
	{

	}

}