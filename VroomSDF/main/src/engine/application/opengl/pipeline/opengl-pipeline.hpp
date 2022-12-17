#pragma once

#include "../../../../core/graphics-wrapper.hpp"
#include "../../../../core/glm-wrapper.hpp"
#include "../../../../core/sdl-wrapper.hpp"
#include <string>
#include <vector>

namespace vrm {
	class OpenGLPipeline {
	public:
		OpenGLPipeline(SDL_Window* window, const std::string& shaderName);
		void SetMousePosition(Sint32 mouseX, Sint32 mouseY);
		void Render();
		~OpenGLPipeline();
	private:
		SDL_Window* window;
		Sint32 mousePosition[2] = { 0,0 };
		const GLuint shaderProgramId;

		const GLuint uniformLocationCounter;
		const GLuint uniformLocationiResolution;
		const GLuint uniformLocationCamera;

		const GLuint attributeLocationVertexPosition;

		const GLsizei stride;
		const GLsizei offsetPosition;

		const GLuint ScreenSurfaceVertexBuffer;
		const GLuint ScreenSurfaceIndexBuffer;
	};
}