#include "vertex.hpp"

using vrm::Vertex;

bool Vertex::operator==(const Vertex& other) const {
	return position == other.position && texCoord == other.texCoord;
}