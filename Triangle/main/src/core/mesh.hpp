#pragma once

#include "vertex.hpp"
#include <vector>

namespace vrm {
	class Mesh {
	public:
		Mesh(const std::vector<vrm::Vertex> meshVertices, 
			const std::vector<uint32_t> meshIndices);
		const std::vector<vrm::Vertex> GetVertices() const;
		const std::vector<uint32_t> GetIndices() const;
	private:
		const std::vector<vrm::Vertex> vertices;
		const std::vector<uint32_t> indices;
	};
}