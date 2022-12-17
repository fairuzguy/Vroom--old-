#pragma once

#include <iostream>

#ifndef NDEBUG
#ifdef __ANDROID__
#include <android/log.h>
#endif
#endif

namespace vrm {
	void print(const std::string& message);
	void printErr(const std::string& message, const std::exception& error);
}