#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/mesh.hpp"

namespace vrm {
	class OpenGLMesh {
	public:
		OpenGLMesh(const vrm::Mesh& mesh);
		const GLuint& GetVertexBufferId() const;
		const GLuint& GetIndexBufferId() const;
		const uint32_t& GetNumIndices() const;
		~OpenGLMesh();
	private:
		const GLuint bufferIdVertices;
		const GLuint bufferIdIndices;
		const uint32_t numIndices;
	};
}