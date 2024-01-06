#include "mulpch.h"
#include "Muelsyse/Renderer/GraphicsContext.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLContext.h"

namespace mul
{
	Scope<GraphicsContext> GraphicsContext::create(void* window)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}