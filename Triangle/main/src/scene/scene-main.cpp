#include "scene-main.hpp"
#include "../core/perspective-camera.hpp"
#include "../core/static-mesh-instance.hpp"
#include "../core/asset-manager.hpp"

using vrm::SceneMain;
using vrm::assets::Pipeline;
using vrm::assets::StaticMesh;
using vrm::assets::Texture;

namespace {
	vrm::PerspectiveCamera createCamera(const float& width, const float& height) {
		return vrm::PerspectiveCamera(width, height);
	}
}

SceneMain::SceneMain(const float& screenWidth, const float& screenHeight) :
	camera(::createCamera(screenWidth, screenHeight)) {
}

void SceneMain::Prepare(vrm::AssetManager& assetManager) {
	assetManager.LoadPipelines({ Pipeline::Default });
	assetManager.LoadStaticMeshes({ StaticMesh::Crate });
	assetManager.LoadTextures({ Texture::Crate });

	staticMeshes.push_back(vrm::StaticMeshInstance{ StaticMesh::Crate, Texture::Crate });
}

void SceneMain::Update(const float& delta) {
	const glm::mat4 cameraMatrix(camera.GetProjectionMatrix() * camera.GetViewMatrix());
	for (auto& staticMesh : staticMeshes) {
		staticMesh.RotateBy(delta * 45.0f);
		staticMesh.Update(cameraMatrix);
	}
}

void SceneMain::Render(vrm::Renderer& renderer) {
	renderer.Render(Pipeline::Default, staticMeshes);
}