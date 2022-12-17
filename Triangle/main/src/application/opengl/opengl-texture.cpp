#include "../../core/graphics-wrapper.hpp"
#include "opengl-texture.hpp"

using vrm::OpenGLTexture;

namespace {
	GLuint createTexture(const vrm::Bitmap& bitmap) {
		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			bitmap.GetWidth(),
			bitmap.GetHeight(),
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bitmap.GetPixelData()
		);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureId;
	}
}

OpenGLTexture::OpenGLTexture(const vrm::Bitmap& bitmap) :
	textureId(::createTexture(bitmap)) {
}

void OpenGLTexture::Bind() const {
	glBindTexture(GL_TEXTURE_2D, textureId);
}

OpenGLTexture::~OpenGLTexture() {
	glDeleteTextures(1, &textureId);
}