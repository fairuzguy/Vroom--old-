#pragma once

#include "asset-inventory.hpp"
#include "../application/opengl/opengl-asset-manager.hpp"
#include <vector>

namespace vrm {
	class AssetManager {
	public:
		void LoadPipelines(const std::vector<vrm::assets::Pipeline>& piplines);
		void LoadStaticMeshes(const std::vector<vrm::assets::StaticMesh>& staticMeshes);
		void LoadTextures(const std::vector<vrm::assets::Texture>& textures);
	};
}