#pragma once

#include "../../core/asset-manager.hpp"
#include "opengl-mesh.hpp"
#include "opengl-pipeline.hpp"
#include "opengl-texture.hpp"
#include <unordered_map>

namespace vrm {
	class OpenGLAssetManager {
	public:
		OpenGLAssetManager();
		void LoadPipelines(const std::vector<vrm::assets::Pipeline>& pipelines);
		void LoadStaticMeshes(const std::vector<vrm::assets::StaticMesh>& staticMeshes);
		void LoadTextures(const std::vector<vrm::assets::Texture>& textures);
		const vrm::OpenGLPipeline& GetPipeline(const vrm::assets::Pipeline& pipeline) const;
		const vrm::OpenGLMesh& GetStaticMesh(const vrm::assets::StaticMesh& staticMesh) const;
		const vrm::OpenGLTexture& GetTexture(const vrm::assets::Texture& texture) const;
	private:
		std::unordered_map<vrm::assets::Pipeline, vrm::OpenGLPipeline> pipelineCache;
		std::unordered_map<vrm::assets::StaticMesh, vrm::OpenGLMesh> staticMeshCache;
		std::unordered_map<vrm::assets::Texture, vrm::OpenGLTexture> textureCache;
	};
}