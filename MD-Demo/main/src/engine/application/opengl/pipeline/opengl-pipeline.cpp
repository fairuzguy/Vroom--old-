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

glm::mat3 fromAxisAngle(const glm::vec3 axis, const float angle) {
	float cosT = glm::cos(angle);
	float sinT = glm::sin(angle);
	glm::mat3 rotationMatrix = {
		cosT + axis.x * axis.x * (1 - cosT),
		axis.x * axis.y * (1 - cosT) - axis.z * sinT,
		axis.x * axis.z * (1 - cosT) + axis.y * sinT,
		axis.x * axis.y * (1 - cosT) + axis.z * sinT,
		cosT + axis.y * axis.y * (1 - cosT),
		axis.y * axis.z * (1 - cosT) - axis.x * sinT,
		axis.x * axis.z * (1 - cosT) - axis.y * sinT,
		axis.y * axis.z * (1 - cosT) + axis.x * sinT,
		cosT + axis.z * axis.z * (1 - cosT)
	};
	return rotationMatrix;
}

void printMatrix3(const glm::mat3 matrix) {
	std::cout <<
		matrix[0][0] << ", " << matrix[0][1] << ", " << matrix[0][2] << "\n" <<
		matrix[1][0] << ", " << matrix[1][1] << ", " << matrix[1][2] << "\n" <<
		matrix[2][0] << ", " << matrix[2][1] << ", " << matrix[2][2] << std::endl;
}

GLfloat* PrepareObjects(const std::vector<glm::mat4> ObjectData) {
	GLfloat* u_ObjectData = new GLfloat[16 * ObjectData.size()];
	for (int i = 0; i < ObjectData.size(); i++) {
		for (int j = 0; j < 4; j++) {
			u_ObjectData[16 * i + j*4] = ObjectData[i][j].x;
			u_ObjectData[16 * i + j*4 + 1] = ObjectData[i][j].y;
			u_ObjectData[16 * i + j*4 + 2] = ObjectData[i][j].z;
			u_ObjectData[16 * i + j*4 + 3] = ObjectData[i][j].w;
		}
	}
	return u_ObjectData;
}

uint32_t MouseMarch(const std::vector<glm::mat4> ObjectData, const glm::vec2 mousePosition, const glm::vec2 iResolution, const glm::mat4 cameraMatrix) {
	glm::vec3 cameraPosition = { cameraMatrix[0].x, cameraMatrix[0].y, cameraMatrix[0].z };
	float cameraZoom = cameraMatrix[0].w;
	glm::vec3 rightVector = { cameraMatrix[1].x, cameraMatrix[1].y, cameraMatrix[1].z };
	glm::vec3 upVector = { cameraMatrix[2].x, cameraMatrix[2].y, cameraMatrix[2].z };
	glm::vec3 lookVector = { cameraMatrix[3].x, cameraMatrix[3].y, cameraMatrix[3].z };
	glm::vec2 uv = {
		(mousePosition.x - 0.5 * iResolution.x) / iResolution.y,
		-(mousePosition.y - 0.5 * iResolution.y) / iResolution.y
	};

	glm::vec3 rayOrigin = cameraPosition;
	glm::vec3 rayDirection = glm::normalize(glm::vec3(
		cameraZoom*lookVector + uv.x*rightVector + uv.y*upVector
	));
	
	float DistTravelled = 0;
	int ObjectIdentifier = 100000000;
	float finalDist = 200000000000;
	for (int j = 0; j < 100; j++) {
		glm::vec3 currentPosition = rayOrigin + rayDirection * DistTravelled;
		float minDist = 10000000000;
		for (int i = 0; i < ObjectData.size(); i++) {
			glm::vec4 sphere = ObjectData[i][0];
			glm::vec3 position = { sphere.x, sphere.y, sphere.z };
			float radius = sphere.w;
			float sphereDist = glm::length(currentPosition - position) - radius;
			if (sphereDist < minDist && sphereDist < 0.1) {
				ObjectIdentifier = i;
			}
			minDist = glm::min(sphereDist, minDist);
		}
		DistTravelled += minDist;
		if (DistTravelled > 100 || minDist < 0.01) {
			finalDist = minDist;
			break;
		}
	}
	//vrm::print(std::to_string(ObjectIdentifier));
	//std::cout << uv.x << ", " << uv.y << std::endl;
	return ObjectIdentifier;
}

void ComputePhysics(std::vector<glm::mat4> &ObjectData, float dt, int currentObject) {
	// using a simple spring based (but clamped) method (euler approx for now, other methods will try later)
	// first one is fixed because of numerical problems with the 10 million approximations
	dt = glm::clamp<double>(dt, 0.0, 0.0167); // keeping dt steady but not limited the framerate
	const double k = 0.5;
	const double m = 1;
	const double B = 0.05;
	const double MAX_DISTANCE = 5;
	const double REST = 0;
	std::vector<glm::mat4> ObjectCache = ObjectData;
	glm::vec3 r1(0);
	glm::vec3 r2(0);
	for (int i = 0; i < ObjectData.size(); i++) {
		if (i == 0) continue;
		if (i == currentObject) continue;
		glm::vec3 velocity = { ObjectData[i][1].x, ObjectData[i][1].y, ObjectData[i][1].z };
		glm::vec3 position = { ObjectData[i][0].x, ObjectData[i][0].y, ObjectData[i][0].z };
		if (i > 0) { // previous neighbour
			glm::vec3 prevPosition = { ObjectData[i - 1][0].x, ObjectData[i - 1][0].y, ObjectData[i - 1][0].z };
			r1 = position - prevPosition;
		}
		if (i < ObjectData.size() - 1) { // next neighbour
			glm::vec3 nextPosition = { ObjectData[i + 1][0].x, ObjectData[i + 1][0].y, ObjectData[i + 1][0].z };
			r2 = position - nextPosition;
		}
		glm::vec3 R0 = r1 + r2;
		R0 = glm::clamp(R0, glm::vec3(-MAX_DISTANCE), glm::vec3(MAX_DISTANCE));
		//std::cout << R0.x << ", " << R0.y << ", " << R0.z << std::endl;
		glm::vec3 springForce = {
			-k / m * (R0.x - REST),
			-k / m * (R0.y - REST),
			-k / m * (R0.z - REST)
		};
		glm::vec3 newPosition = {
			0.5 * dt * dt * (-k / m * (R0.x - REST) - B / m * velocity.x) + (1-B)*dt * velocity.x + position.x,
			0.5 * dt * dt * (-k / m * (R0.y - REST) - B / m * velocity.y) + (1-B)*dt * velocity.y + position.y,
			0.5 * dt * dt * (-k / m * (R0.z - REST) - B / m * velocity.z) + (1-B)*dt * velocity.z + position.z,
		};
		//std::cout << i << ", " << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << ", " << std::endl;
		glm::vec3 newVelocity = (newPosition - position) / dt;
		//std::cout << i << ", " << springForce.x << ", " << springForce.y << ", " << springForce.z << ", " << std::endl;
		//std::cout << i << ", " << currentObject << ", " << dt << ", " << currentObject << ", " << std::endl;
		ObjectCache[i][0] = glm::vec4(newPosition.x, newPosition.y, newPosition.z, ObjectData[i][0].w);
		ObjectCache[i][1] = glm::vec4(newVelocity.x, newVelocity.y, newVelocity.z, 0);
		//std::cout << springForce.x << ", " << springForce.y << ", " << springForce.z << std::endl;
	}
	//std::cout << dt << std::endl;
	ObjectData = ObjectCache;
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
		uniformLocationState(glGetUniformLocation(shaderProgramId, "u_state")),
		uniformLocationObjectData(glGetUniformLocation(shaderProgramId, "u_ObjectInfo")),
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

	void OpenGLPipeline::PassEvents(const std::vector<SDL_Event> events) {
		eventList = events;
	}

	GLfloat PreviousTick = 0;
	bool rightHeldDown = false;
	bool leftHeldDownFirstTime = false;
	bool leftHeldDown = false;
	bool wDown = false;
	bool aDown = false;
	bool sDown = false;
	bool dDown = false;
	float cachedMousePosition[2] = { 0,0 };
	glm::vec2 cachedObjectMousePosition(0);
	int currentObject = 10000000;
	glm::mat3 cachedCameraOrientation;
	GLfloat cameraMatrix[16] = {
			2, 2, -5, 1,
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0
	};
	glm::mat3 cameraOrientationMovement = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

	std::vector<glm::mat4> ObjectData = { // spheres for now
		glm::mat4(2, 7, 7, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(3, 6, 7, 0.5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(-2, 3, 3, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(1, 7, 9, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(3, 5, 1, 0.5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(-4, 1, 3, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(2.2, 7.7, 2.7, 0.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(3.5, 6, 7, 0.5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
		glm::mat4(-4, 3.3, -2, 0.5, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
	};
	void OpenGLPipeline::Render() {
		glEnableVertexAttribArray(attributeLocationVertexPosition);

		// timer
		GLfloat counter = (SDL_GetTicks()) / 1000.;
		glUniform1fv(uniformLocationCounter, 1, &counter);
		GLfloat dt = counter - PreviousTick;
		//print("Total: " + std::to_string(counter) + ", step: " + std::to_string(dt*1000.));
		PreviousTick = counter;

		// screen updates
		GLfloat iResolution[4] = { 640,480,0,0 };
		int currentWidth = iResolution[0];
		int currentHeight = iResolution[1];
		SDL_GetWindowSize(window, &currentWidth, &currentHeight);
		iResolution[0] = static_cast<GLfloat>(currentWidth);
		iResolution[1] = static_cast<GLfloat>(currentHeight);
		iResolution[2] = static_cast<GLfloat>(mousePosition[0]);
		iResolution[3] = static_cast<GLfloat>(mousePosition[1]);
		glUniform4fv(uniformLocationiResolution, 1, iResolution);
		glViewport(0, 0, static_cast<GLsizei>(currentWidth), static_cast<GLsizei>(currentHeight));

		// preparing objects
		GLint u_state[4] = { ObjectData.size(), 0, 0, 0 };
		glUniform4iv(uniformLocationState, 1, u_state);
		GLfloat* PreparedObjectData = PrepareObjects(ObjectData);
		glUniformMatrix4fv(uniformLocationObjectData, ObjectData.size(), false, PreparedObjectData);

		// camera + events
		GLfloat cameraZoom = cameraMatrix[3];
		glm::vec3 cameraPosition = { cameraMatrix[0], cameraMatrix[1], cameraMatrix[2] };
		glm::mat3 cameraOrientation = {
			cameraMatrix[4], cameraMatrix[5], cameraMatrix[6],
			cameraMatrix[8], cameraMatrix[9], cameraMatrix[10],
			cameraMatrix[12], cameraMatrix[13], cameraMatrix[14]
		};
		glm::vec3 cameraPositionMovement(0);
		bool noRotationFlag = false;
		for (int i = 0; i < eventList.size(); i++) {
			switch (eventList[i].type) {
			case (SDL_MOUSEBUTTONDOWN):
				switch (eventList[i].button.button) {
				case (SDL_BUTTON_LEFT): // selection
					if (!leftHeldDown) {
						leftHeldDownFirstTime = true;
					}
					else leftHeldDownFirstTime = false;
					leftHeldDown = true;
					break;
				case (SDL_BUTTON_RIGHT): // camera movement toggle
					if (!rightHeldDown) {
						cachedMousePosition[0] = mousePosition[0];
						cachedMousePosition[1] = mousePosition[1];
						cachedCameraOrientation = glm::mat3(
							cameraMatrix[4], cameraMatrix[5], cameraMatrix[6],
							cameraMatrix[8], cameraMatrix[9], cameraMatrix[10],
							cameraMatrix[12], cameraMatrix[13], cameraMatrix[14]
						);
						SDL_SetRelativeMouseMode(SDL_TRUE);
					}
					rightHeldDown = true;
					break;
				}
				break;
			case (SDL_MOUSEBUTTONUP):
				switch (eventList[i].button.button) {
				case (SDL_BUTTON_LEFT): // unselection
					leftHeldDownFirstTime = false;
					leftHeldDown = false;
					break;
				case (SDL_BUTTON_RIGHT): // stop camera movement
					if (rightHeldDown) {
						SDL_SetRelativeMouseMode(SDL_FALSE);
					}
					cachedCameraOrientation = glm::mat3(
						cameraMatrix[4], cameraMatrix[5], cameraMatrix[6],
						cameraMatrix[8], cameraMatrix[9], cameraMatrix[10],
						cameraMatrix[12], cameraMatrix[13], cameraMatrix[14]
					);
					//printMatrix3(cachedCameraOrientation);
					rightHeldDown = false;
					break;
				}
				break;
			case (SDL_MOUSEMOTION): { // apply to camera orientation
				//glm::vec2 rotation(eventList[i].motion.yrel, eventList[i].motion.xrel);
				glm::vec2 rotation( // temporary, this doesnt work well 360, only within limited field of vision
					eventList[i].motion.y - cachedMousePosition[1],
					eventList[i].motion.x - cachedMousePosition[0]
				);
				//print(std::to_string(rotation.x) + ", " + std::to_string(rotation.y));
				//print(std::to_string(cachedMousePosition[1]) + ", " + std::to_string(cachedMousePosition[0]));
				float rotationMagnitude = glm::sqrt(rotation.x * rotation.x + rotation.y * rotation.y);
				glm::vec3 axis;
				float angle;
				if (rotationMagnitude <= 0.1) {
					rotation = glm::vec2(0, 0);
					axis = glm::vec3(0, 1, 0);
					angle = 0;
					noRotationFlag = true;
				}
				else {
					axis = glm::vec3(-rotation.x, -rotation.y, 0) / rotationMagnitude;
					angle = rotationMagnitude / 500;
					noRotationFlag = false;
					//print(std::to_string(angle));
				}
				glm::mat3 cameraRotation = fromAxisAngle(axis, angle);
				cameraOrientationMovement = cameraRotation;
				//printMatrix3(cameraRotation);
				break;
			}
			case (SDL_KEYDOWN): // move camera
				switch (eventList[i].key.keysym.sym) {
				case(SDLK_w):
					wDown = true;
					break;
				case(SDLK_a):
					aDown = true;
					break;
				case(SDLK_s):
					sDown = true;
					break;
				case(SDLK_d):
					dDown = true;
					break;
				}
				break;
			case (SDL_KEYUP): // stop moving camera
				switch (eventList[i].key.keysym.sym) {
				case(SDLK_w):
					wDown = false;
					break;
				case(SDLK_a):
					aDown = false;
					break;
				case(SDLK_s):
					sDown = false;
					break;
				case(SDLK_d):
					dDown = false;
					break;
				}
				break;
			}
		}
		if (rightHeldDown) { // sloppy and doesn't look great either.........
			//SDL_WarpMouseInWindow(window, cachedMousePosition[0], cachedMousePosition[1]);
			//printMatrix3(cameraOrientationMovement);
			cameraOrientation = cameraOrientationMovement*cachedCameraOrientation;
			//cameraOrientation = glm::transpose(cameraOrientation);
			cameraOrientation[0] = glm::normalize(cameraOrientation[0]);
			cameraOrientation[1] = glm::normalize(cameraOrientation[1]);
			cameraOrientation[2] = glm::normalize(cameraOrientation[2]);
			cameraMatrix[4] = cameraOrientation[0][0];
			cameraMatrix[5] = cameraOrientation[0][1];
			cameraMatrix[6] = cameraOrientation[0][2];
			cameraMatrix[8] = cameraOrientation[1][0];
			cameraMatrix[9] = cameraOrientation[1][1];
			cameraMatrix[10] = cameraOrientation[1][2];
			cameraMatrix[12] = cameraOrientation[2][0];
			cameraMatrix[13] = cameraOrientation[2][1];
			cameraMatrix[14] = cameraOrientation[2][2];
		}
		glm::vec3 RightVector = { cameraMatrix[4], cameraMatrix[5], cameraMatrix[6] };
		glm::vec3 UpVector = { cameraMatrix[8], cameraMatrix[9], cameraMatrix[10] };
		glm::vec3 LookVector = { cameraMatrix[12], cameraMatrix[13], cameraMatrix[14] };
		//printMatrix3(glm::mat3(RightVector, UpVector, LookVector));
		int speedCounter = 0;
		float speedMultiplier = 3*dt;
		if (wDown) {
			speedCounter++;
			cameraPositionMovement += LookVector* speedMultiplier;
		}
		if (aDown) {
			speedCounter++;
			cameraPositionMovement -= RightVector* speedMultiplier;
		}
		if (sDown) {
			speedCounter++;
			cameraPositionMovement -= LookVector* speedMultiplier;
		}
		if (dDown) {
			speedCounter++;
			cameraPositionMovement += RightVector * speedMultiplier;
		}
		if (speedCounter > 1) { // normalizing movement for uniform speed
			cameraPositionMovement /= glm::sqrt(speedCounter);
		}
		cameraMatrix[0] += cameraPositionMovement.x;
		cameraMatrix[1] += cameraPositionMovement.y;
		cameraMatrix[2] += cameraPositionMovement.z;
		eventList.clear();
		glUniformMatrix4fv(uniformLocationCamera, 1, false, cameraMatrix);

		// dragging objects
		uint32_t objectIndex = MouseMarch(
			ObjectData, glm::vec2(mousePosition[0], mousePosition[1]), 
			glm::vec2(iResolution[0], iResolution[1]), glm::mat4(
				cameraMatrix[0], cameraMatrix[1], cameraMatrix[2], cameraMatrix[3],
				cameraMatrix[4], cameraMatrix[5], cameraMatrix[6], cameraMatrix[7],
				cameraMatrix[8], cameraMatrix[9], cameraMatrix[10], cameraMatrix[11],
				cameraMatrix[12], cameraMatrix[13], cameraMatrix[14], cameraMatrix[15]
			)
		);
		if (leftHeldDownFirstTime) {
			leftHeldDownFirstTime = false;
			currentObject = objectIndex;
			cachedObjectMousePosition = glm::vec2(mousePosition[0], mousePosition[1]);
		}
		if (!leftHeldDown) {
			currentObject = 10000000000000;
		}
		if (currentObject <= ObjectData.size()) {
			glm::vec2 k = glm::vec2(mousePosition[0], mousePosition[1]) - cachedObjectMousePosition;
			k = glm::vec2(k.x, -k.y);
			k /= 20;
			k = glm::clamp(k, glm::vec2(-0.3), glm::vec2(0.3));
			//print(std::to_string(k.x) + ", " + std::to_string(k.y));
			glm::vec3 ObjectPosition = glm::vec3(ObjectData[currentObject][0].x, ObjectData[currentObject][0].y, ObjectData[currentObject][0].z);
			ObjectPosition = ObjectPosition + k.x * RightVector + k.y * UpVector;
			ObjectData[currentObject][0] = glm::vec4(ObjectPosition.x, ObjectPosition.y, ObjectPosition.z, ObjectData[currentObject][0].w);
			cachedObjectMousePosition = glm::vec2(mousePosition[0], mousePosition[1]);
		}
		//print(std::to_string(currentObject));
		//std::cout << ObjectData[5][0].x << ", " << ObjectData[5][0].y << ", " << ObjectData[5][0].z << ", " << std::endl;
		ComputePhysics(ObjectData, dt, currentObject);


		glDrawElements(
			GL_TRIANGLES,
			3,
			GL_UNSIGNED_INT,
			(GLvoid*) 0
		);
		glDisableVertexAttribArray(attributeLocationVertexPosition);
		delete[] PreparedObjectData;
	}

	OpenGLPipeline::~OpenGLPipeline() {
		glDeleteProgram(shaderProgramId);
	}
}