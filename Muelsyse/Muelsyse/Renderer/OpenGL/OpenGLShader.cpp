#include "mulpch.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

namespace mul
{
	OpenGLShader::OpenGLShader(const std::string& vertex, const std::string& fragment, bool isFromFile)
	{
		MUL_PROFILE_FUNCTION();

		unsigned int vert, frag;
		m_RendererID = glCreateProgram();

		if (isFromFile)
		{
			std::string vertexSrc, fragmentSrc;
			vertexSrc = m_ReadFile(vertex);
			fragmentSrc = m_ReadFile(fragment);

			vert = m_CompileShader(vertexSrc, ShaderType::Vertex);
			frag = m_CompileShader(fragmentSrc, ShaderType::Fragment);
		}
		else
		{
			vert = m_CompileShader(vertex, ShaderType::Vertex);
			frag = m_CompileShader(fragment, ShaderType::Fragment);
		}

		glAttachShader(m_RendererID, vert);
		glAttachShader(m_RendererID, frag);
		glLinkProgram(m_RendererID);

		int status, length;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
			char* msg = (char*)_malloca(length * sizeof(char));
			glGetProgramInfoLog(m_RendererID, length, &length, msg);
			MUL_CORE_ERROR("Error: shader link error.");
			MUL_CORE_ERROR(msg);
		}
		
		glDetachShader(m_RendererID, vert);
		glDetachShader(m_RendererID, frag);
		
		glDeleteShader(vert);
		glDeleteShader(frag);
	}

	OpenGLShader::~OpenGLShader()
	{
		MUL_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::bind() const
	{
		MUL_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::unBind() const
	{
		MUL_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, int value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformInt(name, value);
	}

	void OpenGLShader::setIntArray(const std::string& name, int* values, uint32_t count)
	{
		setUniformIntArray(name, values, count);
	}

	void OpenGLShader::setFloat(const std::string& name, const float value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformFloat(name, value);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformFloat3(name, value);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformFloat4(name, value);
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformMat4(name, value);
	}

	void OpenGLShader::setUniformInt(const std::string& name, int value)
	{
		glUniform1i(m_GetUniformLocation(name), value);
	}

	void OpenGLShader::setUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::setUniformFloat(const std::string& name, float value)
	{
		glUniform1f(m_GetUniformLocation(name), value);
	}

	void OpenGLShader::setUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		glUniform2f(m_GetUniformLocation(name), value.x, value.y);
	}

	void OpenGLShader::setUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		glUniform3f(m_GetUniformLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::setUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		glUniform4f(m_GetUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::setUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(m_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::setUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(m_GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::m_ReadFile(const std::string& filepath)
	{
		MUL_PROFILE_FUNCTION();

		// Read source
		std::ifstream file(std::filesystem::absolute(filepath).generic_string(), std::ios_base::in);
		if (!file.is_open())
		{
			MUL_CORE_ERROR("Error: Shader file open error.");
			MUL_ASSERT(false, "Shader file open error.")
			file.close();
			return 0;
		}

		std::string line;
		std::string shader;
		while (std::getline(file, line))
			shader += line + "\n";
		
		return shader;
	};

	unsigned int OpenGLShader::m_CompileShader(const std::string& src, ShaderType type)
	{
		MUL_PROFILE_FUNCTION();

		const char* shaderSrc = src.c_str();
		// Compile shader
		unsigned int shaderID;
		shaderID = glCreateShader((type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER));
		glShaderSource(shaderID, 1, &shaderSrc, NULL);
		glCompileShader(shaderID);

		// Check error
		int status, length;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
			char* msg = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(shaderID, length, &length, msg);
			MUL_CORE_ERROR("Error: {0} shader compile error.", (type == ShaderType::Vertex ? "Vertex" : "Fragment"));
			MUL_CORE_ERROR(msg);
			glDeleteShader(shaderID);
			return 0;
		}

		return shaderID;
	};

	unsigned int OpenGLShader::m_GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_RendererID, name.c_str());
	};
}