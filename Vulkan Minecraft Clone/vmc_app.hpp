#pragma once
#include "vmc_window.hpp"
#include "vmc_pipeline.hpp"
#include "vmc_device.hpp"

namespace vmc {
	class VmcApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();
	private:
		VmcWindow vmcWindow{ WIDTH, HEIGHT, "Hello Vulkan!" };
		VmcDevice vmcDevice{ vmcWindow };
		VmcPipeline vmcPipeline{vmcDevice, "../Shaders/simple_shader.vert.spv", "../Shaders/simple_shader.frag.spv", VmcPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
	};
}

