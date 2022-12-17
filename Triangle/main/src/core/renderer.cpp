#include "renderer.hpp"

using vrm::Renderer;

Renderer::Renderer(vrm::RendererType rendererType) :
	rendererType(rendererType) {
}

void Renderer::Render(
	const vrm::assets::Pipeline& pipeline,
	const std::vector<vrm::StaticMeshInstance>& staticMeshInstances) {
	switch (rendererType) {
	case vrm::RendererType::OpenGL:
		openGLRenderer->Render(pipeline, staticMeshInstances);
		break;
	case vrm::RendererType::Vulkan:
		break;
	case vrm::RendererType::Metal:
		break;
	case vrm::RendererType::Direct3D:
		break;
	}
}