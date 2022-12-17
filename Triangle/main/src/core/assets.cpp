#define TINYOBJLOADER_IMPLEMENTATION

#include "assets.hpp"
#include "sdl-wrapper.hpp"
#include "vertex.hpp"
#include <sstream>
#include <tiny_obj_loader.h>
#include <unordered_map>
#include <vector>
#include "log.hpp"
#include <SDL_image.h>

std::string vrm::assets::LoadTextFile(const std::string& path) {
	SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r");
	if (file == nullptr) {
		vrm::log("File loading error for " + path, SDL_GetError());
		throw std::runtime_error("Some error with reading an asset file.");
	}
	size_t fileLength = static_cast<size_t>(SDL_RWsize(file));
	void* data = SDL_LoadFile_RW(file, nullptr, 1);
	std::string result(static_cast<char*>(data), fileLength);
	SDL_free(data);
	return result;
}

vrm::Mesh vrm::assets::LoadOBJFile(const std::string& path) {
	std::istringstream sourceStream(vrm::assets::LoadTextFile(path));

	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(
		&attributes, &shapes, &materials, &warning, &error, &sourceStream)) {
		throw std::runtime_error("vrm::assets::loadOBJFile: Error: " + warning + error);
	}

	std::vector<vrm::Vertex> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<vrm::Vertex, uint32_t> uniqueVertices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			glm::vec3 position{
				attributes.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 0],
				attributes.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 1],
				attributes.vertices[3 * static_cast<uint64_t>(index.vertex_index) + 2]
			};
			glm::vec2 texCoord{
				attributes.texcoords[2*index.texcoord_index + 0],
				1.0f - attributes.texcoords[2*index.texcoord_index + 1]
			};
			vrm::Vertex vertex{ position, texCoord };
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vrm::Vertex{ position, texCoord });
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
	vrm::log("Count", std::to_string(vertices.size()) + ", " +
		std::to_string(indices.size()));
	return vrm::Mesh{ vertices, indices };
}

vrm::Bitmap vrm::assets::LoadBitmap(const std::string& path) {
	SDL_RWops* file = SDL_RWFromFile(path.c_str(), "rb");
	SDL_Surface* source = IMG_Load_RW(file, 1);
	SDL_Rect imageFrame = { 0, 0, source->w, source->h };

	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
	uint32_t alphaMask;

#if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	redMask = 0xff000000;
	greenMask = 0x00ff0000;
	blueMask = 0x0000ff00;
	alphaMask = 0x000000ff;
#else
	redMask = 0x000000ff;
	greenMask = 0x0000ff00;
	blueMask = 0x00ff0000;
	alphaMask = 0xff000000;
#endif
	SDL_Surface* target = SDL_CreateRGBSurface(
		0,
		imageFrame.w, imageFrame.h,
		32,
		redMask, greenMask, blueMask, alphaMask
	);
	SDL_BlitSurface(source, &imageFrame, target, &imageFrame);
	SDL_FreeSurface(source);
	return vrm::Bitmap(target);
}