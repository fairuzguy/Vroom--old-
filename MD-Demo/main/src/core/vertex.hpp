#pragma once

#include "glm-wrapper.hpp"

namespace vrm {
	struct Vertex {
		glm::vec3 position;
		glm::vec2 texCoord;

		bool operator==(const vrm::Vertex& other) const;
	};
}

namespace std {
	template <>
	struct hash<vrm::Vertex> {
		size_t operator()(const vrm::Vertex& vertex) const {
			return ((hash<glm::vec3>()(vertex.position) ^ 
				(hash<glm::vec2>()(vertex.texCoord) << 1)) >> 1);
		}
	};
}
