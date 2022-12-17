#pragma once

#include "mesh/mesh.hpp"
#include "texture/bitmap.hpp"
#include <string>

namespace vrm::assets {
	std::string LoadTextFile(const std::string& path);
	vrm::Mesh LoadOBJFile(const std::string& path);
	vrm::Bitmap LoadBitmap(const std::string& path);
}