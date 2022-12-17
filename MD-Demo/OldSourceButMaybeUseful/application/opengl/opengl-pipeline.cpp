#include "opengl-pipeline.hpp"
#include "../../core/assets.hpp"
#include "../../core/log.hpp"
#include "opengl-asset-manager.hpp"
#include <stdexcept>
#include <vector>

using vrm::OpenGLPipeline;

namespace {
	GLuint compileShader(const GLenum& shaderType, const std::string& shaderSource) {
		const std::string logTag = "vrm::OpenGLPipeline::compileShader";
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
			vrm::log(logTag, &errorMessage[0]);
			throw std::runtime_error(logTag + "Shader failed to compile.");
		}
		return shaderId;
	}

	GLuint createShaderProgram(const std::string& shaderName) {
		const std::string logTag = "vrm::OpenGLPipeline::createShaderProgram";
		vrm::log(logTag, "Creating pipeline for '" + shaderName + "'");
		const std::string vertexShaderCode = vrm::assets::LoadTextFile(
			"assets/shaders/opengl/" + shaderName + ".vert"
		);
		const std::string fragmentShaderCode = vrm::assets::LoadTextFile(
			"assets/shaders/opengl/" + shaderName + ".frag"
		);

#ifdef USING_GLES
		std::string vertexShaderSource = "#version 100\n" + vertexShaderCode;
		std::string fragmentShaderSource = "#version 100\nprecision mediump float;\n" + fragmentShaderCode;
#else
		std::string vertexShaderSource = "#version 120\n" + vertexShaderCode;
		std::string fragmentShaderSource = "#version 120\n" + fragmentShaderCode;
#endif
		GLuint shaderProgramId = glCreateProgram();
		GLuint vertexShaderId = ::compileShader(GL_VERTEX_SHADER, vertexShaderSource);
		GLuint fragmentShaderId = ::compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

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
			vrm::log(logTag, &errorMessage[0]);
			throw std::runtime_error(logTag + "Shader program failed to compile.");
		}

		glDetachShader(shaderProgramId, vertexShaderId);
		glDetachShader(shaderProgramId, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return shaderProgramId;
	}
}

OpenGLPipeline::OpenGLPipeline(const std::string& shaderName) :
	shaderProgramId(::createShaderProgram(shaderName)),
	uniformLocationMVP(glGetUniformLocation(shaderProgramId, "u_mvp")),
	attributeLocationVertexPosition(glGetAttribLocation(shaderProgramId, "a_vertexPosition")),
	attributeLocationTexCoord(glGetAttribLocation(shaderProgramId, "a_texCoord")),
	stride(5 * sizeof(float)),
	offsetPosition(0),
	offsetTexCoord(3*sizeof(float)) {
}

void OpenGLPipeline::Render(
	const vrm::OpenGLAssetManager& assetManager,
	const std::vector<vrm::StaticMeshInstance>& staticMeshInstances) const {
	glUseProgram(shaderProgramId);
	glEnableVertexAttribArray(attributeLocationVertexPosition);
	glEnableVertexAttribArray(attributeLocationTexCoord);

	for (const auto& staticMeshInstance : staticMeshInstances) {
		const vrm::OpenGLMesh& mesh = assetManager.GetStaticMesh(staticMeshInstance.GetMesh());
		glUniformMatrix4fv(uniformLocationMVP, 1, GL_FALSE, &staticMeshInstance.GetTransformMatrix()[0][0]);
		assetManager.GetTexture(staticMeshInstance.GetTexture()).Bind();
		glBindBuffer(GL_ARRAY_BUFFER, mesh.GetVertexBufferId());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetIndexBufferId());

		glVertexAttribPointer(
			attributeLocationVertexPosition,
			3, GL_FLOAT, GL_FALSE, stride, 
			reinterpret_cast<const GLvoid*>(offsetPosition)
		);
		glVertexAttribPointer(
			attributeLocationTexCoord, 2, GL_FLOAT,
			GL_FALSE, stride, 
			reinterpret_cast<const GLvoid*>(offsetTexCoord)
		);
		glDrawElements(
			GL_TRIANGLES,
			mesh.GetNumIndices(),
			GL_UNSIGNED_INT,
			reinterpret_cast<const GLvoid*>(0)
		);
	}
	glDisableVertexAttribArray(attributeLocationVertexPosition);
	glDisableVertexAttribArray(attributeLocationTexCoord);
}

OpenGLPipeline::~OpenGLPipeline() {
	glDeleteProgram(shaderProgramId);
}