#include "print.hpp"

namespace vrm {
	void print(const std::string& message) {
#ifndef NDEBUG
#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_DEBUG, "Vroom", "%s: %s", "Debug: ", message.c_str());
#else
		std::cout << message << std::endl;
#endif
#endif
	}

	void printErr(const std::string& message, const std::exception& error) {
#ifndef NDEBUG
		std::string output = message + " Exception message was: " + std::string{ error.what() };
		vrm::print(output);
#endif
	}
}