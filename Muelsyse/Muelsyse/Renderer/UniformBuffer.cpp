#include "mulpch.h"
#include "Muelsyse/Renderer/UniformBuffer.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLUniformBuffer.h"

namespace mul 
{
	Ref<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLUniformBuffer>(size, binding);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}