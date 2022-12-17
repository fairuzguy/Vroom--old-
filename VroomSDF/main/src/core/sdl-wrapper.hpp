#pragma once

#include <SDL.h>
#include <utility>

namespace vrm::sdl {
	std::pair<uint32_t, uint32_t> getDisplaySize();
	SDL_Window* createWindow(const uint32_t& windowFlags);
}
