#include "engine.hpp"
#include "../debug/print.hpp"
#include "../core/sdl-wrapper.hpp"
#include <stdexcept>
#include <SDL_image.h>
#ifdef _WIN32
	#include <windows.h>
	#include <SDL_syswm.h>
#endif

using vrm::Engine;

Engine::Engine() {
	vrm::print("Initializing Vroom...");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
#ifdef _WIN32
	SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, "1");
#endif
	vrm::print("SDL2 initialized successfully! Resolving application type (OpenGL, Vulkan, ...)");
	application = ResolveApplication();
	vrm::print("Application resolved!");

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		throw std::runtime_error("Could not initialize SDL2_image.");
	}
	vrm::print("SDL2_image initialized successfully with PNG support!");
}

void Engine::Run() {
	vrm::print("Starting application...");
	application->StartApplication();
}

std::unique_ptr<vrm::Application> Engine::ResolveApplication() {
	try {
		vrm::print("Creating OpenGL application");
		return std::make_unique<vrm::Application>(vrm::ApplicationType::OpenGL);
	}
	catch (const std::exception& error) {
		vrm::printErr("OpenGL application failed to initialize.", error);
	}
	throw std::runtime_error("No applications can run in this environment!");
}

void Engine::Quit() {
	SDL_Quit();
}
