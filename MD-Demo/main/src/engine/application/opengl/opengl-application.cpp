#include "opengl-application.hpp"
#include "../../../assets/assets.hpp"
#include "../../../debug/print.hpp"
#include "../../../core/perspective-camera.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32
HWND workerw;
BOOL EnumWindowsProc(HWND hwnd, LPARAM lparam) {
	HWND p = FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL);
	if (p != NULL) {
		workerw = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
	}

	return true;
}
#endif

SDL_Window* CreateNewWindow() {
	SDL_Window* window;
#ifdef _WIN32
	HWND progman = FindWindow("Progman", nullptr);
	WPARAM yes(0);
	UINT flags = 0x0000;
	UINT timeout = 1000;
	PDWORD_PTR result(0);
	SendMessageTimeout(progman, 0x052C, yes, NULL, flags, timeout, result);

	BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lparam);
	BOOL(*wndNumProcFuncPtr)(HWND hwnd, LPARAM lparam);
	wndNumProcFuncPtr = EnumWindowsProc;
	EnumWindows(wndNumProcFuncPtr, NULL);

	HDC dc = NULL; //GetDCEx(workerw, NULL, DCX_WINDOW); // trying to render to desktop background...
	std::cout << workerw << std::endl;
	if (dc != NULL) {
		window = SDL_CreateWindowFrom((void*)workerw);
	}
	else {
		window = vrm::sdl::createWindow(
			SDL_WINDOW_OPENGL |
			SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_ALLOW_HIGHDPI
		);
	}
#else
	window = vrm::sdl::createWindow(
		SDL_WINDOW_OPENGL |
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_ALLOW_HIGHDPI
	);
#endif
	return window;
}

SDL_GLContext CreateContext(SDL_Window* window) {
	SDL_GLContext newContext = SDL_GL_CreateContext(window);
#ifdef _WIN32
	glewInit();
#endif
	int viewportWidth;
	int viewportHeight;
	SDL_GL_GetDrawableSize(window, &viewportWidth, &viewportHeight);
	vrm::print("Created OpenGL context with viewport size: " + std::to_string(viewportWidth) + " x " + std::to_string(viewportHeight));

	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glViewport(0, 0, viewportWidth, viewportHeight);

	return newContext;
}

namespace vrm {
	OpenGLApplication::OpenGLApplication() :
		window(CreateNewWindow()),
		context(::CreateContext(window)),
		openGLPipeline(std::make_unique<vrm::OpenGLPipeline>(window, "default")) {
	}

	void OpenGLApplication::SetMousePosition(Sint32 mouseX, Sint32 mouseY) {
		openGLPipeline->SetMousePosition(mouseX, mouseY);
	}

	void OpenGLApplication::PassEvents(const std::vector<SDL_Event> events) {
		openGLPipeline->PassEvents(events);
	}

	void OpenGLApplication::Render() {
		SDL_GL_MakeCurrent(window, context);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		vrm::PerspectiveCamera tempCamera(vrm::sdl::getDisplaySize().first, vrm::sdl::getDisplaySize().second);
		const glm::mat4 mvp = glm::mat4(tempCamera.GetProjectionMatrix());
		openGLPipeline->Render();

		SDL_GL_SwapWindow(window);
	}

	OpenGLApplication::~OpenGLApplication() {
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
	}
}