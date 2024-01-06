#include "mulpch.h"
#include "Muelsyse/Renderer/Shader.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

namespace mul 
{
	Ref<Shader> Shader::create(const std::string& vertex, const std::string& fragment, bool isFromFile)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLShader>(vertex, fragment, isFromFile);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}