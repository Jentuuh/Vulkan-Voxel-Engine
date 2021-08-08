#pragma once

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE	// Forces that glm uses depth values in the [0;1] range, unlike the standard [-1;1] range.
#include <glm/glm.hpp>

namespace vmc {
	class VmcCamera
	{
	public:
		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fov_vert, float aspect, float near, float far);

		const glm::mat4& getProjection() const { return projectionMatrix; };

	private:
		glm::mat4 projectionMatrix{ 1.f };
	};



}
