#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../../core/sdl-wrapper.hpp"
#include "application.hpp"
#include "../../debug/print.hpp"
#include <memory>

#ifdef __EMSCRIPTEN__
void emscriptenMainLoop(vrm::Application* application) {
	application->RunMainLoop();
}
#endif

namespace vrm {
	Application::Application(ApplicationType applicationType) :
	applicationType(applicationType) {
		// Resolve application to OpenGL (soon Vulkan, Metal, D3D12)
		if (applicationType == vrm::ApplicationType::OpenGL) {
			openGLApplication = std::make_unique<vrm::OpenGLApplication>();
		}
	}

	void Application::StartApplication() {
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg((em_arg_callback_func) ::emscriptenMainLoop, this, 60, 1);
#else
		while (RunMainLoop()) {
		}
#endif
	}

	bool Application::RunMainLoop() {
		SDL_Event event;
		std::vector<SDL_Event> events;
		while (SDL_PollEvent(&event)) {
			events.push_back(event);
			switch (event.type) {
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					return false;
				}
				break;
			case SDL_MOUSEMOTION:
				openGLApplication->SetMousePosition(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:

				break;
			default:
				break;
			}
		}
		PassEvents(events);
		RenderApplication();
		return true;
	}

	void Application::PassEvents(const std::vector<SDL_Event> events) {
		openGLApplication->PassEvents(events);
	}

	void Application::RenderApplication() {
		switch (applicationType) {
		case vrm::ApplicationType::OpenGL:
			openGLApplication->Render();
			break;
		case vrm::ApplicationType::Vulkan:
			break;
		case vrm::ApplicationType::Metal:
			break;
		case vrm::ApplicationType::Direct3D:
			break;
		}
	}
}