#pragma once

namespace vrm {
	enum class Platform {
		mac,
		ios,
		android,
		emscripten,
		windows
	};

	Platform getCurrentPlatform();
}
