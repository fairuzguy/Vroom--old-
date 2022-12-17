#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "../core/sdl-wrapper.hpp"
#include "application.hpp"
#include "opengl/opengl-application.hpp"
#include "../core/log.hpp"
#include <memory>

using vrm::Application;

namespace {
#ifdef __EMSCRIPTEN__
	void emscriptenMainLoop(vrm::Application* application) {
		application->RunMainLoop();
	}
#endif
}

Application::Application(vrm::ApplicationType resolvingApplicationType) :
	performanceFrequency(static_cast<float>(SDL_GetPerformanceFrequency())),
	currentTime(SDL_GetPerformanceCounter()),
	previousTime(currentTime) {
	// Resolve application to OpenGL (soon Vulkan, Metal, D3D12)
	applicationType = resolvingApplicationType;
	if (resolvingApplicationType == vrm::ApplicationType::OpenGL) {
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
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				return false;
			}
			break;
		default:
			break;
		}
	}
	UpdateApplication(TimeStep());
	RenderApplication();
	return true;
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

void Application::UpdateApplication(const float& delta) {
	switch (applicationType) {
	case vrm::ApplicationType::OpenGL:
		openGLApplication->Update(delta);
		break;
	case vrm::ApplicationType::Vulkan:
		break;
	case vrm::ApplicationType::Metal:
		break;
	case vrm::ApplicationType::Direct3D:
		break;
	}
}

float Application::TimeStep() {
	previousTime = currentTime;
	currentTime = SDL_GetPerformanceCounter();
	float elapsed = (currentTime - previousTime) * 1000.0f;
	return elapsed / performanceFrequency * 0.001f;
}