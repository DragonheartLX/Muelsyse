#include "mulpch.h"
#include "Muelsyse/Renderer/RendererAPI.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace mul
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createScope<OpenGLRendererAPI>();
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}