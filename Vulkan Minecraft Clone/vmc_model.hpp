#pragma once

#include "vmc_device.hpp"
#include "chunk_component.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE	// Forces that glm uses depth values in the [0;1] range, unlike the standard [-1;1] range.
#include <glm/glm.hpp>

// std 
#include <memory>
#include <vector>

namespace vmc {
	class VmcModel
	{
	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const {
				return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
			}
		};

		struct Builder {
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filePath);
			void updateChunkMesh(ChunkComponent chunk);
		};

		VmcModel(VmcDevice &device, const VmcModel::Builder &builder);
		~VmcModel();

		VmcModel(const VmcModel&) = delete;
		VmcModel& operator=(const VmcModel&) = delete;

		static std::unique_ptr<VmcModel> createModelFromFile(VmcDevice& device, const std::string& filePath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);


	private:
		void createVertexBuffers(const std::vector<Vertex> &vertices);
		void createIndexBuffers(const std::vector<uint32_t> &indices);


		VmcDevice& vmcDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer = false;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};
}
