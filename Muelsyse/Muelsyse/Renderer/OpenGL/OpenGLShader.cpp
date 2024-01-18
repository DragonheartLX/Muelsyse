#include "mulpch.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"
#include "Muelsyse/Core/Timer.h"

#include <fstream>
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace mul
{
	namespace Utils 
	{
		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			MUL_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
				case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
			}
			MUL_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(GLenum stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
				case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
			}
			MUL_CORE_ASSERT(false);
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/opengl";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		{
			switch (stage)
			{
				case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
				case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
			}
			MUL_CORE_ASSERT(false);
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		{
			switch (stage)
			{
			case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
			}
			MUL_CORE_ASSERT(false);
			return "";
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filepath):
		m_FilePath(filepath)
	{
		MUL_PROFILE_FUNCTION();

		Utils::CreateCacheDirectoryIfNeeded();

		std::string source = m_ReadFile(filepath);
		auto shaderSources = m_PreProcess(source);

		{
			Timer timer;
			m_CompileOrGetVulkanBinaries(shaderSources);
			m_CompileOrGetOpenGLBinaries();
			m_CreateProgram();
			MUL_CORE_WARN("Shader creation took {0} ms", timer.elapsedMillis());
		}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc): 
		m_Name(name)
	{
		MUL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;

		m_CompileOrGetVulkanBinaries(sources);
		m_CompileOrGetOpenGLBinaries();
		m_CreateProgram();
	}

	OpenGLShader::~OpenGLShader()
	{
		MUL_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::m_ReadFile(const std::string& filepath)
	{
		MUL_PROFILE_FUNCTION();

		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				MUL_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			MUL_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::m_PreProcess(const std::string& source)
	{
		MUL_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			MUL_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			MUL_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			MUL_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void OpenGLShader::m_CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					MUL_CORE_ERROR(module.GetErrorMessage());
					MUL_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data]: shaderData)
			m_Reflect(stage, data);
	}

	void OpenGLShader::m_CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		const bool optimize = false;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		shaderData.clear();
		m_OpenGLSourceCode.clear();
		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedOpenGLFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();
				auto& source = m_OpenGLSourceCode[stage];

				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str());
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					MUL_CORE_ERROR(module.GetErrorMessage());
					MUL_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}
	}

	void OpenGLShader::m_CreateProgram()
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto&& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}

		glLinkProgram(program);

		GLint isLinked;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
			MUL_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_FilePath, infoLog.data());

			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}

	void OpenGLShader::m_Reflect(unsigned int stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();
		
		MUL_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		MUL_CORE_TRACE(">> {0} uniform buffers", resources.uniform_buffers.size());
		MUL_CORE_TRACE(">> {0} resources", resources.sampled_images.size());
		MUL_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			MUL_CORE_TRACE(">> {0}", resource.name);
			MUL_CORE_TRACE(">> Size = {0}", bufferSize);
			MUL_CORE_TRACE(">> Binding = {0}", binding);
			MUL_CORE_TRACE(">> Members = {0}", memberCount);
		}
		MUL_CORE_TRACE("=====================");
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

	void OpenGLShader::setFloat2(const std::string& name, const glm::vec2& value)
	{
		MUL_PROFILE_FUNCTION();

		setUniformFloat2(name, value);
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

	unsigned int OpenGLShader::m_GetUniformLocation(const std::string& name)
	{
		return glGetUniformLocation(m_RendererID, name.c_str());
	};
}