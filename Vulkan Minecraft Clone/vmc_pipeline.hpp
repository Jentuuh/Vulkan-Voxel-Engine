#pragma once

#include "vmc_device.hpp"

// std 
#include <string>
#include <vector>

namespace vmc {

	struct PipelineConfigInfo 
	{

	};

	class VmcPipeline
	{
	public: 
		VmcPipeline(VmcDevice& device, const std::string vertFilePath, const std::string fragFilePath, const PipelineConfigInfo& configInfo );
		~VmcPipeline(){}

		VmcPipeline(const VmcPipeline&) = delete;
		VmcPipeline operator=(const VmcPipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
	private:
		static std::vector<char> readFile(const std::string& filePath);
		void createGraphicsPipeline(const std::string vertFilePath, const std::string fragFilePath, const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		VmcDevice& vmcDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

	};

}
