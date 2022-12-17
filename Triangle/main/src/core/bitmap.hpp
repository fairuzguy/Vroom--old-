#pragma once

#include "sdl-wrapper.hpp"

namespace vrm {
	class Bitmap {
	public:
		Bitmap(SDL_Surface* surface);
		uint16_t GetWidth() const;
		uint16_t GetHeight() const;
		void* GetPixelData() const;
		~Bitmap();
	private:
		SDL_Surface* surface;
	};
}