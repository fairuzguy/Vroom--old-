#pragma once

#include "../../core/graphics-wrapper.hpp"
#include "../../core/glm-wrapper.hpp"
#include "../../core/static-mesh-instance.hpp"
#include <string>
#include <vector>

namespace vrm {
	class OpenGLAssetManager; // avoiding circular dependency

	class OpenGLPipeline {
	public:
		OpenGLPipeline(const std::string& shaderName);
		void Render(
			const vrm::OpenGLAssetManager& assetManager, 
			const std::vector<vrm::StaticMeshInstance>& staticMeshInstances
		) const;
		~OpenGLPipeline();
	private:
		const GLuint shaderProgramId;
		const GLuint uniformLocationMVP;
		const GLuint attributeLocationVertexPosition;
		const GLuint attributeLocationTexCoord;
		const GLsizei stride;
		const GLsizei offsetPosition;
		const GLsizei offsetTexCoord;
	};
}
