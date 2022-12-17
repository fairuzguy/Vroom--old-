#include "static-mesh-instance.hpp"

using vrm::StaticMeshInstance;

const glm::mat4& identity = glm::mat4(1.0f);

StaticMeshInstance::StaticMeshInstance(
	const vrm::assets::StaticMesh& mesh,
	const vrm::assets::Texture& texture,
	const glm::vec3& position,
	const glm::vec3& scale,
	const glm::vec3& rotationAxis,
	const float& rotationDegrees) :
	mesh(mesh),
	texture(texture),
	position(position),
	scale(scale),
	rotationAxis(rotationAxis),
	rotationDegrees(rotationDegrees) {
}

void StaticMeshInstance::Update(const glm::mat4& projectionViewMatrix) {
	transformMatrix = projectionViewMatrix *
		glm::translate(identity, position) *
		glm::rotate(identity, glm::radians(rotationDegrees), rotationAxis) *
		glm::scale(identity, scale);
}

void StaticMeshInstance::RotateBy(const float& degrees) {
	rotationDegrees += degrees;
	if (rotationDegrees > 360.0f) {
		rotationDegrees -= 360.0f;
	}
	else if (rotationDegrees < -360.0f) {
		rotationDegrees += 360.0f;
	}
}

vrm::assets::StaticMesh StaticMeshInstance::GetMesh() const {
	return mesh;
}

vrm::assets::Texture StaticMeshInstance::GetTexture() const {
	return texture;
}

glm::mat4 StaticMeshInstance::GetTransformMatrix() const {
	return transformMatrix;
}