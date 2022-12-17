#pragma once

#include "../application/application.hpp"
#include <memory>
#include <string>

namespace vrm {
	class Engine {
	public:
		Engine();
		void Run();
	private:
		const std::string classLogTag = "vrm::Engine::";
		std::unique_ptr<vrm::Application> application;

		std::unique_ptr<vrm::Application> ResolveApplication();
		void Quit();
	};
}