#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "sdl-wrapper.hpp"
#include "platform.hpp"

namespace {
	bool shouldDisplayFullScreen() {
		switch (vrm::getCurrentPlatform()) {
			case vrm::Platform::ios:
				return true;
			case vrm::Platform::android:
				return true;
			case vrm::Platform::mac:
			case vrm::Platform::emscripten:
			case vrm::Platform::windows:
				return false;
			defualt:
				return false;
		}
	}
}

std::pair<uint32_t, uint32_t> vrm::sdl::getDisplaySize() {
	uint32_t displayWidth = 0;
	uint32_t displayHeight = 0;
#ifdef __EMSCRIPTEN__
	displayWidth = static_cast<uint32_t>(EM_ASM_INT({
		return document.getElementById('canvas').width;
	}));
	displayHeight = static_cast<uint32_t>(EM_ASM_INT({
		return document.getElementById('canvas').height;
	}));
#else
	switch (vrm::getCurrentPlatform()) {
		case vrm::Platform::ios: {
			SDL_DisplayMode displayMode;
			SDL_GetDesktopDisplayMode(0, &displayMode);
			displayWidth = static_cast<uint32_t>(displayMode.w);
			displayHeight = static_cast<uint32_t>(displayMode.h);
			break;
		}
		case vrm::Platform::android: {
			SDL_DisplayMode displayMode;
			SDL_GetDesktopDisplayMode(0, &displayMode);
			displayWidth = static_cast<uint32_t>(displayMode.w);
			displayHeight = static_cast<uint32_t>(displayMode.h);
			break;
		}
		default: {
			displayWidth = 640;
			displayHeight = 480;
			break;
		}
	}
#endif
	return std::make_pair(displayWidth, displayHeight);
}

SDL_Window* vrm::sdl::createWindow(const uint32_t& windowFlags) {
	std::pair<uint32_t, uint32_t> displaySize = vrm::sdl::getDisplaySize();
	SDL_Window* window = SDL_CreateWindow(
		"Triangle",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		displaySize.first, displaySize.second,
		windowFlags
	);

	if (::shouldDisplayFullScreen()) {
		SDL_SetWindowFullscreen(window, SDL_TRUE);
	}

	return window;
}
