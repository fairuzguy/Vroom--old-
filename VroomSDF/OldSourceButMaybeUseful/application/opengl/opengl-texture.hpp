#pragma once

#include "../../core/bitmap.hpp"

namespace vrm {
	class OpenGLTexture {
	public:
		OpenGLTexture(const vrm::Bitmap& bitmap);
		void Bind() const;
		~OpenGLTexture();
	private:
		const GLuint textureId;
	};
}