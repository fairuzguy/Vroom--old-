#pragma once

#include "asset-inventory.hpp"
#include "glm-wrapper.hpp"

namespace vrm {
	class StaticMeshInstance {
	public:
		StaticMeshInstance(
			const vrm::assets::StaticMesh& staticMesh,
			const vrm::assets::Texture& texture,
			const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f),
			const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
			const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f),
			const float& rotationDegrees = 0.0f
		);
		void Update(const glm::mat4& projectionViewMatrix);
		void RotateBy(const float& degrees);
		vrm::assets::StaticMesh GetMesh() const;
		vrm::assets::Texture GetTexture() const;
		glm::mat4 GetTransformMatrix() const;
	private:
		const vrm::assets::StaticMesh mesh;
		const vrm::assets::Texture texture;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 rotationAxis;
		float rotationDegrees;
		glm::mat4 transformMatrix;
	};
}