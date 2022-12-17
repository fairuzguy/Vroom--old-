#pragma once

#include "../core/asset-manager.hpp"
#include "../core/renderer.hpp"

namespace vrm {
	class Scene {
		Scene();
		void Prepare(vrm::AssetManager& assetManager);
		void Update(const float& delta);
		void Render(vrm::Renderer& renderer);
		~Scene();
	};
}