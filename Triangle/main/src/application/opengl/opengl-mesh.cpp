#include "opengl-mesh.hpp"
#include "../../core/glm-wrapper.hpp"
#include <vector>

using vrm::OpenGLMesh;

namespace {
	GLuint createVertexBuffer(const vrm::Mesh& mesh) {
		std::vector<float> bufferData;
		for (const auto& vertex : mesh.GetVertices()) {
			bufferData.push_back(vertex.position.x);
			bufferData.push_back(vertex.position.y);
			bufferData.push_back(vertex.position.z);

			bufferData.push_back(vertex.texCoord.x);
			bufferData.push_back(vertex.texCoord.y);
		}
		GLuint bufferId;
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ARRAY_BUFFER, bufferId);
		glBufferData(
			GL_ARRAY_BUFFER,
			bufferData.size() * sizeof(float),
			bufferData.data(),
			GL_STATIC_DRAW
		);
		return bufferId;
	}

	GLuint createIndexBuffer(const vrm::Mesh& mesh) {
		GLuint bufferId;
		glGenBuffers(1, &bufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			mesh.GetIndices().size() * sizeof(uint32_t),
			mesh.GetIndices().data(),
			GL_STATIC_DRAW
		);
		return bufferId;
	}
}

OpenGLMesh::OpenGLMesh(const vrm::Mesh& mesh) :
	bufferIdVertices(::createVertexBuffer(mesh)),
	bufferIdIndices(::createIndexBuffer(mesh)),
	numIndices(static_cast<uint32_t>(mesh.GetIndices().size())) {
}

const GLuint& OpenGLMesh::GetVertexBufferId() const {
	return bufferIdVertices;
}

const GLuint& OpenGLMesh::GetIndexBufferId() const {
	return bufferIdIndices;
}

const uint32_t& OpenGLMesh::GetNumIndices() const {
	return numIndices;
}

OpenGLMesh::~OpenGLMesh() {
	glDeleteBuffers(1, &bufferIdVertices);
	glDeleteBuffers(1, &bufferIdIndices);
}