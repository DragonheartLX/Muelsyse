#include "mulpch.h"
#include "Muelsyse/Renderer/Texture.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLTexture.h"

namespace mul
{
	Ref<Texture2D> Texture2D::create(const TextureSpecification& specification, Buffer data)
	{
		switch (Renderer::getAPI())
		{
			case RendererAPI::API::None:    MUL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return createRef<OpenGLTexture2D>(specification, data);
		}

		MUL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}