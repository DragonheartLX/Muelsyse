#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Renderer/Shader.h"

#include <glm/glm.hpp>

namespace mul
{
	class MUL_API OpenGLShader: public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual void setInt(const std::string& name, int value) override;
		virtual void setIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void setFloat(const std::string& name, const float value) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;

		void setUniformInt(const std::string& name, int value);
		void setUniformIntArray(const std::string& name, int* values, uint32_t count);

		void setUniformFloat(const std::string& name, float value);
		void setUniformFloat2(const std::string& name, const glm::vec2& value);
		void setUniformFloat3(const std::string& name, const glm::vec3& value);
		void setUniformFloat4(const std::string& name, const glm::vec4& value);

		void setUniformMat3(const std::string& name, const glm::mat3& matrix);
		void setUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string m_ReadFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> m_PreProcess(const std::string& source);

		unsigned int m_GetUniformLocation(const std::string& name);

		void m_CompileOrGetVulkanBinaries(const std::unordered_map<unsigned int, std::string>& shaderSources);
		void m_CompileOrGetOpenGLBinaries();
		void m_CreateProgram();
		void m_Reflect(unsigned int stage, const std::vector<uint32_t>& shaderData);

		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;

		std::unordered_map<unsigned int, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<unsigned int, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<unsigned int, std::string> m_OpenGLSourceCode;
	};
}