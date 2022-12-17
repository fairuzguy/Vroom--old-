#pragma once

#include "scene.hpp"
#include "../core/perspective-camera.hpp"
#include "../core/static-mesh-instance.hpp"

namespace vrm {
	class SceneMain {
	public:
		SceneMain(const float& screenWidth, const float& screenHeight);
		void Prepare(vrm::AssetManager& assetManager);
		void Update(const float& delta);
		void Render(vrm::Renderer& renderer);
	private:
		vrm::PerspectiveCamera camera;
		std::vector<vrm::StaticMeshInstance> staticMeshes;
	};
}