#pragma once

#include <string>

namespace vrm::assets {
	enum class Pipeline {
		Default
	};
	enum class StaticMesh {
		Crate
	};
	enum class Texture {
		Crate
	};

	std::string ResolvePipelinePath(const vrm::assets::Pipeline& pipeline);
	std::string ResolveStaticMeshPath(const vrm::assets::StaticMesh& staticMesh);
	std::string ResolveTexturePath(const vrm::assets::Texture& texture);
}