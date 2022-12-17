#include "opengl-renderer.hpp"

using vrm::OpenGLRenderer;

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<vrm::OpenGLAssetManager> assetManager) :
	assetManager(assetManager) {
}

void OpenGLRenderer::Render(
	const vrm::assets::Pipeline& pipeline,
	const std::vector<vrm::StaticMeshInstance>& staticMeshInstances) {
	assetManager->GetPipeline(pipeline).RenderMesh(*assetManager, staticMeshInstances);
}
