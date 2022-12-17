#pragma once

#include "opengl-asset-manager.hpp"
#include "../../core/static-mesh-instance.hpp"
#include <memory>

namespace vrm {
	class OpenGLRenderer {
	public:
		OpenGLRenderer(std::shared_ptr<vrm::OpenGLAssetManager> assetManager);
		void Render(
			const vrm::assets::Pipeline& pipeline,
			const std::vector<vrm::StaticMeshInstance>& staticMeshInstances
		);
	private:
		const std::shared_ptr<vrm::OpenGLAssetManager> assetManager;
	};
}