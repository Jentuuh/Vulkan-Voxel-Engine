#pragma once

#include "vmc_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE	// Forces that glm uses depth values in the [0;1] range, unlike the standard [-1;1] range.
#include <glm/glm.hpp>

// std 
#include <vector>

namespace vmc {
	class VmcModel
	{
	public:
		struct Vertex {
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		};

		VmcModel(VmcDevice &device, const std::vector<Vertex>& vertices);
		~VmcModel();

		VmcModel(const VmcModel&) = delete;
		VmcModel& operator=(const VmcModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);

		VmcDevice& vmcDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}
