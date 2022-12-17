#include "engine.hpp"
#include "log.hpp"
#include "sdl-wrapper.hpp"
#include <stdexcept>
#include <SDL_image.h>

using vrm::Engine;

Engine::Engine() {
	static const std::string logTag = classLogTag + "init";
	vrm::log(logTag, "Initializing Vroom...");
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	vrm::log(logTag, "SDL2 initialized successfully! Resolving application type (OpenGL, Vulkan, ...)");
	application = ResolveApplication();
	vrm::log(logTag, "Application resolved!");

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		throw std::runtime_error(logTag + " Could not initialize SDL2_image...");
	}
	vrm::log(logTag, "SDL2_image initialized successfully with PNG support!");
}

void Engine::Run() {
	static const std::string logTag = classLogTag + "run";
	vrm::log(logTag, "Starting application...");
	application->StartApplication();
}

std::unique_ptr<vrm::Application> Engine::ResolveApplication() {
	static const std::string logTag = classLogTag + "resolveApplication";
	try {
		vrm::log(logTag, "Creating OpenGL application");
		return std::make_unique<vrm::Application>(vrm::ApplicationType::OpenGL);
	}
	catch (const std::exception& error) {
		vrm::log(logTag, "OpenGL application failed to initialize.", error);
	}
	throw std::runtime_error(logTag + " No applications can run in this environment!");
}

void Engine::Quit() {
	SDL_Quit();
}
