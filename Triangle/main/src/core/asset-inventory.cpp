#include "asset-inventory.hpp"

std::string vrm::assets::ResolvePipelinePath(const vrm::assets::Pipeline& pipeline) {
	switch (pipeline) {
	case vrm::assets::Pipeline::Default:
		return "default";
	}
}

std::string vrm::assets::ResolveStaticMeshPath(const vrm::assets::StaticMesh& staticMesh) {
	switch (staticMesh) {
	case vrm::assets::StaticMesh::Crate:
		return "assets/models/crate.obj";
	}
}

std::string vrm::assets::ResolveTexturePath(const vrm::assets::Texture& texture) {
	switch (texture) {
	case vrm::assets::Texture::Crate:
		return "assets/textures/crate.png";
	}
}