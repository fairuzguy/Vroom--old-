#pragma once

#include "asset-inventory.hpp"
#include "static-mesh-instance.hpp"
#include "../application/opengl/opengl-renderer.hpp"
#include <vector>

namespace vrm {
	enum class RendererType {
		OpenGL, Vulkan, Metal, Direct3D
	};
	class Renderer {
	public:
		Renderer(vrm::RendererType rendererType);
		void Render(
			const vrm::assets::Pipeline& pipeline,
			const std::vector<vrm::StaticMeshInstance>& staticMeshInstances
		);
	private:
		vrm::RendererType rendererType;
		std::unique_ptr<vrm::OpenGLRenderer> openGLRenderer;
	};
}