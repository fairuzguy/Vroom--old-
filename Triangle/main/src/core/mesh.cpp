#include "mesh.hpp"

using vrm::Mesh;

Mesh::Mesh(const std::vector<vrm::Vertex> meshVertices,
	const std::vector<uint32_t> meshIndices) : 
	vertices(meshVertices), indices(meshIndices) {
}

const std::vector<vrm::Vertex> Mesh::GetVertices() const {
	return vertices;
}

const std::vector<uint32_t> Mesh::GetIndices() const {
	return indices;
}