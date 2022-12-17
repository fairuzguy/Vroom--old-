#pragma once

#include <memory>
#include "opengl/opengl-application.hpp"

namespace vrm {
	enum class ApplicationType {
		OpenGL, Vulkan, Metal, Direct3D
	};

	class Application {
	public:
		Application(ApplicationType applicationType);
		void StartApplication();
		bool RunMainLoop();
		void PassEvents(const std::vector<SDL_Event> events);
		void RenderApplication();
		~Application() = default;
	private:
		ApplicationType applicationType;
		std::unique_ptr<vrm::OpenGLApplication> openGLApplication;
	};
}