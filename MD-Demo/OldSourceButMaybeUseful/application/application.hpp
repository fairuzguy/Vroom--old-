#pragma once

#include "opengl/opengl-application.hpp"
#include <memory>

namespace vrm {
	enum class ApplicationType {
		OpenGL, Vulkan, Metal, Direct3D
	};

	class Application {
		public:
			Application(vrm::ApplicationType resolvingApplicationType);
			void StartApplication();
			bool RunMainLoop();
			void RenderApplication();
			void UpdateApplication(const float& delta);
			~Application() = default;
		private:
			vrm::ApplicationType applicationType;
			std::unique_ptr<vrm::OpenGLApplication> openGLApplication;
			const float performanceFrequency;
			uint64_t currentTime;
			uint64_t previousTime;

			float TimeStep();
	};
}
