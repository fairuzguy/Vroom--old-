#pragma once

#include "../../core/assets.hpp"
#include "../../core/graphics-wrapper.hpp"
#include "../../core/sdl-wrapper.hpp"
#include "../../core/perspective-camera.hpp"
#include "opengl-pipeline.hpp"
#include "opengl-mesh.hpp"
#include "opengl-texture.hpp"

namespace vrm {
	class OpenGLApplication {
	public:
		OpenGLApplication();
		void Render();
		void Update(const float& delta);
		~OpenGLApplication();
	private:
		SDL_Window* window;
		SDL_GLContext context;
		const std::shared_ptr<vrm::OpenGLAssetManager> assetManager;
		vrm::OpenGLRenderer renderer;
		std::unique_ptr<vrm::SceneMain> scene;
	};
}