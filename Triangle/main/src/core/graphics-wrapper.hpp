#pragma once

#if defined(__EMSCRIPTEN__)
	#include <GLES2/gl2.h>
	#define USING_GLES
#elif defined(__APPLE__)
	#define GL_SILENCE_DEPRECATION
	#include "TargetConditionals.h"
	#if (TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
		#include <OpenGLES/ES2/gl.h>
		#define USING_GLES
	#else
		#include <OpenGL/gl3.h>
	#endif
#elif __ANDROID__
	#include <GLES2/gl2.h>
	#define USING_GLES
#elif _WIN32
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif
