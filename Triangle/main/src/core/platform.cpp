#include "platform.hpp"

vrm::Platform vrm::getCurrentPlatform() {
	#if defined(__EMSCRIPTEN__)
		return vrm::Platform::emscripten;
	#elif defined(__APPLE__)
		#include "TargetConditionals.h"
		#if TARGET_OS_IPHONE
			return vrm::Platform::ios;
		#else
			return vrm::Platform::mac;
		#endif
	#elif __ANDROID__
		return vrm::Platform::android;
	#elif _WIN32
		return vrm::Platform::windows;
	#endif
}
