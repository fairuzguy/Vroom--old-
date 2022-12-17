#include "opengl-asset-manager.hpp"
#include "../../core/assets.hpp"

using vrm::OpenGLAssetManager;

OpenGLAssetManager::OpenGLAssetManager() {
}

void OpenGLAssetManager::LoadPipelines(const std::vector<vrm::assets::Pipeline>& pipelines) {
	for (const auto& pipeline : pipelines) {
		if (pipelineCache.count(pipeline) == 0) {
			pipelineCache.insert(std::make_pair(
				pipeline,
				vrm::OpenGLPipeline(vrm::assets::ResolvePipelinePath(pipeline))
			));
		}
	}
}

void OpenGLAssetManager::LoadStaticMeshes(const std::vector<vrm::assets::StaticMesh>& staticMeshes) {
	for (const auto& staticMesh : staticMeshes) {
		if (staticMeshCache.count(staticMesh) == 0) {
			staticMeshCache.insert(std::make_pair(
				staticMesh,
				vrm::OpenGLMesh(vrm::assets::LoadOBJFile(vrm::assets::ResolveStaticMeshPath(staticMesh)))
			));
		}
	}
}

void OpenGLAssetManager::LoadTextures(const std::vector<vrm::assets::Texture>& textures) {
	for (const auto& texture : textures) {
		if (textureCache.count(texture) == 0) {
			textureCache.insert(std::make_pair(
				texture,
				vrm::OpenGLTexture(vrm::assets::LoadBitmap(vrm::assets::ResolveTexturePath(texture)))
			));
		}
	}
}

const vrm::OpenGLPipeline& OpenGLAssetManager::GetPipeline(const vrm::assets::Pipeline& pipeline) const {
	return pipelineCache.at(pipeline);
}

const vrm::OpenGLMesh& OpenGLAssetManager::GetStaticMesh(const vrm::assets::StaticMesh& staticMesh) const {
	return staticMeshCache.at(staticMesh);
}

const vrm::OpenGLTexture& OpenGLAssetManager::GetTexture(const vrm::assets::Texture& texture) const {
	return textureCache.at(texture);
}