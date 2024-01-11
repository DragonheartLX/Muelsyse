#include "mulpch.h"
#include "Muelsyse/Renderer/FrameBuffer.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLFrameBuffer.h"

namespace mul
{
	Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLFramebuffer>(spec);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}