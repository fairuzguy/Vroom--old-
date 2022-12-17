#include "bitmap.hpp"

using vrm::Bitmap;

Bitmap::Bitmap(SDL_Surface* surface) :
	surface(surface) {
}

uint16_t Bitmap::GetWidth() const {
	return static_cast<uint16_t>(surface->w);
}

uint16_t Bitmap::GetHeight() const {
	return static_cast<uint16_t>(surface->h);
}

void* Bitmap::GetPixelData() const {
	return surface->pixels;
}

Bitmap::~Bitmap() {
	SDL_FreeSurface(surface);
}