#include "../../core/assets.hpp"
#include "opengl-application.hpp"
#include "../../core/log.hpp"
#include "../../core/perspective-camera.hpp"
#include "opengl-asset-manager.hpp"
#include "opengl-renderer.hpp"
#include "../../scene/scene-main.hpp"

using vrm::OpenGLApplication;

namespace {
	SDL_GLContext createContext(SDL_Window* window) {
		static const std::string logTag = "ast::OpenGLApplication::createContext";
		SDL_GLContext newContext = SDL_GL_CreateContext(window);
#ifdef _WIN32
		glewInit();
#endif
		int viewportWidth;
		int viewportHeight;
		SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
		vrm::log(logTag, "CreatedOpenGL context with viewport size: " +
			std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));

		glClearDepthf(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glViewport(0, 0, viewportWidth, viewportHeight);

		return newContext;
	}

	std::shared_ptr<vrm::OpenGLAssetManager> createAssetManager() {
		return std::make_shared<vrm::OpenGLAssetManager>(vrm::OpenGLAssetManager());
	}

	vrm::OpenGLRenderer createRenderer(std::shared_ptr<vrm::OpenGLAssetManager> assetManager) {
		return vrm::OpenGLRenderer(assetManager);
	}

	std::unique_ptr<vrm::SceneMain> createMainScene(vrm::AssetManager& assetManager) {
		std::pair<uint32_t, uint32_t> displaySize = vrm::sdl::getDisplaySize();
		std::unique_ptr<vrm::SceneMain> scene = std::make_unique<vrm::SceneMain>(
			static_cast<float>(displaySize.first),
			static_cast<float>(displaySize.second)
		);
		scene->Prepare(assetManager);
		return scene;
	}
}

OpenGLApplication::OpenGLApplication() :
	window(vrm::sdl::createWindow(
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_ALLOW_HIGHDPI
	)),
	context(::createContext(window)),
	assetManager(::createAssetManager()),
	renderer(::createRenderer(assetManager)) {
}

void OpenGLApplication::Render() {
	SDL_GL_MakeCurrent(window, context);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 mvp = glm::mat4(camera.GetProjectionMatrix() *
		camera.GetViewMatrix() * meshTransform);
	defaultPipeline.RenderMesh(mesh, texture, mvp);

	SDL_GL_SwapWindow(window);
}

void OpenGLApplication::Update(const float& delta) {
	// will add more soon
}

OpenGLApplication::~OpenGLApplication() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}
