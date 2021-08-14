#pragma once

// libs
#include <glm/glm.hpp>

// std
#include <vector>

namespace vmc {
	struct BlockModel
	{
		std::vector<glm::vec3> pos_x_face = {
			  {0.5f,0.5f,-0.5f},
			  {0.5f,-0.5f,-0.5f},
			  {0.5f,-0.5f,0.5f},
			  {0.5f,-0.5f,0.5f},
			  {0.5f,0.5f,0.5f},
			  {0.5f,0.5f,-0.5f}
		};

		std::vector<glm::vec3> neg_x_face = {
			{-0.5f,0.5f,-0.5f},
			{-0.5f,-0.5f,-0.5},
			{-0.5f,-0.5f,0.5f},
			{-0.5f,-0.5f,0.5f},
			{-0.5f,0.5f,0.5f},
			{-0.5f,0.5f,-0.5f},
		};

		std::vector<glm::vec3> pos_y_face = {
			{-0.5f,0.5f,0.5f},
			{-0.5f,0.5f,-0.5f},
			{0.5f,0.5f,-0.5f},
			{0.5f,0.5f,-0.5f},
			{0.5f,0.5f,0.5f},
			{-0.5f,0.5f,0.5f},
		};

		std::vector<glm::vec3> neg_y_face = {
			{-0.5f,-0.5f,0.5f},
			{-0.5f,-0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f},
			{0.5f,-0.5f,0.5f},
			{-0.5f,-0.5f,0.5f},
		};

		std::vector<glm::vec3> pos_z_face = {
			{-0.5f,0.5f,0.5f},
			{-0.5f,-0.5f,0.5f},
			{0.5f,-0.5f,0.5f},
			{0.5f,-0.5f,0.5f},
			{0.5f,0.5f,0.5f},
			{-0.5f,0.5f,0.5f},
		};

		std::vector<glm::vec3> neg_z_face = {
			{-0.5f,0.5f,-0.5f},
			{-0.5f,-0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f},
			{0.5f,-0.5f,-0.5f},
			{0.5f,0.5f,-0.5f},
			{-0.5f,0.5f,-0.5f},
		};

		std::vector<glm::vec2> uvs{
			{0.f, 0.f},
			{0.f, 1.f},
			{1.f, 1.f},
			{1.f, 1.f},
			{1.f, 0.f},
			{0.f, 0.f},
		};

		std::vector<glm::vec3> normals{
			{1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f},
			{0.f, 0.f, 1.f},
			{-1.f, 0.f, 0.f},
			{0.f, -1.f, 0.f},
			{0.f, 0.f, -1.f},
		};
	};
}

