#include "perspective-camera.hpp"

using vrm::PerspectiveCamera;

namespace {
	glm::mat4 createProjectionMatrix(const float& width, const float& height) {
		return glm::perspective(glm::radians(60.0f), width / height, 0.01f, 100.0f);
	}

	glm::mat4 createViewMatrix() {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
		glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		return glm::lookAt(position, target, up);
	}
}

PerspectiveCamera::PerspectiveCamera(const float& width, const float& height) :
	projectionMatrix(::createProjectionMatrix(width, height)),
	viewMatrix(::createViewMatrix()) {
}

const glm::mat4& PerspectiveCamera::GetProjectionMatrix() const {
	return projectionMatrix;
}

const glm::mat4& PerspectiveCamera::GetViewMatrix() const {
	return viewMatrix;
}