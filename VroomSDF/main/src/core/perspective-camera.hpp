#pragma once

#include "glm-wrapper.hpp"

namespace vrm {
	class PerspectiveCamera {
	public:
		PerspectiveCamera(const float& width, const float& height);
		const glm::mat4& GetProjectionMatrix() const;
		const glm::mat4& GetViewMatrix() const;
	private:
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
	};
}