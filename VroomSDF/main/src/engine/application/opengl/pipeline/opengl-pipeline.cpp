#include "opengl-pipeline.hpp"
#include "../../../../debug/print.hpp"
#include "../../../../assets/assets.hpp"
#include <stdexcept>
#include <vector>

GLuint CompileShader(const GLenum& shaderType, const std::string& shaderSource) {
	GLuint shaderId = glCreateShader(shaderType);
	const char* shaderData = shaderSource.c_str();
	glShaderSource(shaderId, 1, &shaderData, nullptr);
	glCompileShader(shaderId);
	GLint shaderCompilationResult;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompilationResult);

	if (!shaderCompilationResult) {
		GLint errorMessageLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &errorMessageLength);
		std::vector <char> errorMessage(errorMessageLength + 1);
		glGetShaderInfoLog(shaderId, errorMessageLength, nullptr, &errorMessage[0]);
		vrm::print("Error compiling OpenGL shader:");
		vrm::print(&errorMessage[0]);
		throw std::runtime_error("Shader failed to compile.");
	}
	return shaderId;
}

GLuint CreateShaderProgram(const std::string& shaderName) {
	vrm::print("Creating pipeline for '" + shaderName + "'");
	const std::string vertexShaderCode = vrm::assets::LoadTextFile(
		"assets/shaders/opengl/" + shaderName + ".vert"
	);
	const std::string fragmentShaderCode = vrm::assets::LoadTextFile(
		"assets/shaders/opengl/" + shaderName + ".frag"
	);

	GLint NumFragUniformComponents;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &NumFragUniformComponents);
	GLint NumMaxObjects = (NumFragUniformComponents - 100) / 28;
	std::string ObjectDataUniformDeclaration = "uniform mat4 u_ObjectData[" + std::to_string(NumMaxObjects) + "];\n";
	std::string ObjectParamsUniformDeclaration = "uniform mat4x3 u_ObjectParams[" + std::to_string(NumMaxObjects) + "];\n";

#ifdef USING_GLES
	std::string vertexShaderSource = "#version 100\n" + vertexShaderCode;
	std::string fragmentShaderSource = "#version 100\nprecision mediump float;\n" + 
		ObjectDataUniformDeclaration + ObjectParamsUniformDeclaration + fragmentShaderCode;
#else
	std::string vertexShaderSource = "#version 120\n" + vertexShaderCode;
	std::string fragmentShaderSource = "#version 120\n" + 
		ObjectDataUniformDeclaration + ObjectParamsUniformDeclaration + fragmentShaderCode;
#endif
	GLuint shaderProgramId = glCreateProgram();
	GLuint vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);
	glLinkProgram(shaderProgramId);

	GLint shaderProgramLinkResult;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &shaderProgramLinkResult);

	if (!shaderProgramLinkResult) {
		GLint errorMessageLength;
		glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &errorMessageLength);
		std::vector<char> errorMessage(errorMessageLength + 1);
		glGetProgramInfoLog(shaderProgramId, errorMessageLength, nullptr, &errorMessage[0]);
		vrm::print(&errorMessage[0]);
		throw std::runtime_error("Shader program failed to compile.");
	}

	glDetachShader(shaderProgramId, vertexShaderId);
	glDetachShader(shaderProgramId, fragmentShaderId);
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);

	return shaderProgramId;
}

GLuint CreateVertexBuffer(const vrm::Mesh& mesh) {
	std::vector<float> bufferData;
	for (const auto& vertex : mesh.GetVertices()) {
		bufferData.push_back(vertex.position.x);
		bufferData.push_back(vertex.position.y);
		bufferData.push_back(vertex.position.z);

		bufferData.push_back(vertex.texCoord.x);
		bufferData.push_back(vertex.texCoord.y);
	}
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(
		GL_ARRAY_BUFFER,
		bufferData.size() * sizeof(float),
		bufferData.data(),
		GL_STATIC_DRAW
	);
	return bufferId;
}

GLuint CreateIndexBuffer(const vrm::Mesh& mesh) {
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		mesh.GetIndices().size() * sizeof(uint32_t),
		mesh.GetIndices().data(),
		GL_STATIC_DRAW
	);
	return bufferId;
}

namespace vrm {
	/* Two triangles/one quad
	const std::vector<Vertex> ScreenSurfaceVertices{{
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},{
			glm::vec3(-1.0f, 1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},{
			glm::vec3(1.0f, -1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},{
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		}
	};
	const std::vector<uint32_t> ScreenSurfaceIndices{2,1,0,2,3,1};
	*/
	// Renders one triangle to rasterize instead of 2 - this prevents fragment overlap
	const std::vector<Vertex> ScreenSurfaceVertices{{
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},{
			glm::vec3(-1.0f, 3.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		},{
			glm::vec3(3.0f, -1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f)
		}
	};
	const std::vector<uint32_t> ScreenSurfaceIndices{ 2,1,0 };
	const Mesh ScreenSurface(ScreenSurfaceVertices, ScreenSurfaceIndices);
	const float StartTime = SDL_GetPerformanceCounter();

	OpenGLPipeline::OpenGLPipeline(SDL_Window* window, const std::string& shaderName) :
		window(window),
		shaderProgramId(CreateShaderProgram(shaderName)),
		uniformLocationCounter(glGetUniformLocation(shaderProgramId, "u_counter")),
		uniformLocationiResolution(glGetUniformLocation(shaderProgramId, "u_iResolution")),
		uniformLocationCamera(glGetUniformLocation(shaderProgramId, "u_camera")),
		attributeLocationVertexPosition(glGetAttribLocation(shaderProgramId, "a_vertexPosition")),
		stride(5 * sizeof(float)),
		offsetPosition(0),
		ScreenSurfaceVertexBuffer(CreateVertexBuffer(ScreenSurface)),
		ScreenSurfaceIndexBuffer(CreateIndexBuffer(ScreenSurface)) {
		glUseProgram(shaderProgramId);
		glBindBuffer(GL_ARRAY_BUFFER, ScreenSurfaceVertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ScreenSurfaceIndexBuffer);
		glVertexAttribPointer(
			attributeLocationVertexPosition,
			3, GL_FLOAT, GL_FALSE, stride,
			reinterpret_cast<const GLvoid*>(offsetPosition)
		);
		GLint size;
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &size);
		vrm::print("GL_MAX_UNIFORM_BLOCK_SIZE is " + std::to_string(size) + " bytes.");
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &size);
		vrm::print("GL_MAX_FRAGMENT_UNIFORM_BLOCKS is " + std::to_string(size) + ".");
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &size);
		vrm::print("GL_MAX_FRAGMENT_UNIFORM_VECTORS is " + std::to_string(size) + ".");
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &size);
		vrm::print("GL_MAX_FRAGMENT_UNIFORM_COMPONENTS is " + std::to_string(size) + ".");
		glGetIntegerv(GL_MAX_SUBROUTINES, &size);
		vrm::print("GL_MAX_SUBROUTINES is " + std::to_string(size) + ".");
		glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &size);
		vrm::print("GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS is " + std::to_string(size) + ".");
	}

	void OpenGLPipeline::SetMousePosition(Sint32 mouseX, Sint32 mouseY) {
		mousePosition[0] = mouseX;
		mousePosition[1] = mouseY;
	}

	GLfloat PreviousTick = 0;
	void OpenGLPipeline::Render() {
		glEnableVertexAttribArray(attributeLocationVertexPosition);

		// timer
		GLfloat counter = (SDL_GetTicks())/1000.;
		glUniform1fv(uniformLocationCounter, 1, &counter);
		GLfloat dt = counter - PreviousTick;
		//print("Total: " + std::to_string(counter) + ", step: " + std::to_string(dt*1000.));
		PreviousTick = counter;

		// screen updates
		GLfloat iResolution[4] = {640,480,0,0};
		int currentWidth = iResolution[0];
		int currentHeight = iResolution[1];
		SDL_GetWindowSize(window, &currentWidth, &currentHeight);
		iResolution[0] = static_cast<GLfloat>(currentWidth);
		iResolution[1] = static_cast<GLfloat>(currentHeight);
		iResolution[2] = static_cast<GLfloat>(mousePosition[0]);
		iResolution[3] = static_cast<GLfloat>(mousePosition[1]);
		glUniform4fv(uniformLocationiResolution, 1, iResolution);
		glViewport(0, 0, static_cast<GLsizei>(currentWidth), static_cast<GLsizei>(currentHeight));

		// camera
		GLfloat cameraMatrix[16] = {
			0, 0, 3, 1,
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0
		};
		glUniformMatrix4fv(uniformLocationCamera, 1, true, cameraMatrix);

		glDrawElements(
			GL_TRIANGLES,
			3,
			GL_UNSIGNED_INT,
			(GLvoid*) 0
		);
		glDisableVertexAttribArray(attributeLocationVertexPosition);
	}

	OpenGLPipeline::~OpenGLPipeline() {
		glDeleteProgram(shaderProgramId);
	}
}