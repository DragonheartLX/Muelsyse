#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace mul
{
	enum ShaderType
	{
		Vertex,
		Fragment
	};

	class MUL_API OpenGLShader: public Shader
	{
	public:
		OpenGLShader(const std::string& vertex, const std::string& fragment, bool isFromFile = true);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unBind() const override;

		void setUniformInt(const std::string& name, int value);

		void setUniformFloat(const std::string& name, float value);
		void setUniformFloat2(const std::string& name, const glm::vec2& value);
		void setUniformFloat3(const std::string& name, const glm::vec3& value);
		void setUniformFloat4(const std::string& name, const glm::vec4& value);

		void setUniformMat3(const std::string& name, const glm::mat3& matrix);
		void setUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string m_ReadFile(const std::string& filepath);
		unsigned int m_CompileShader(const std::string& path, ShaderType type);
		unsigned int m_GetUniformLocation(const std::string& name);

		uint32_t m_RendererID;
	};
}