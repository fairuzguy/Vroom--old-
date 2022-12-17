#pragma once

#include "../../../core/graphics-wrapper.hpp"
#include "../../../core/sdl-wrapper.hpp"
#include "../../../core/perspective-camera.hpp"
#include "pipeline/opengl-pipeline.hpp"

namespace vrm {
	class OpenGLApplication {
	public:
		OpenGLApplication();
		void SetMousePosition(Sint32 mouseX, Sint32 mouseY);
		void Render();
		~OpenGLApplication();
	private:
		SDL_Window* window;
		SDL_GLContext context;
		std::unique_ptr<OpenGLPipeline> openGLPipeline;
	};
}