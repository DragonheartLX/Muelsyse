#include "mulpch.h"
#include "Muelsyse/Renderer/Shader.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

namespace mul 
{
	Ref<Shader> Shader::create(const std::string& filepath)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(filepath);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}