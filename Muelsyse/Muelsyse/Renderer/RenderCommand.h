#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Renderer/RendererAPI.h"

namespace mul
{
	class MUL_API RenderCommand
	{
	public:
		inline static void init()
		{
			s_RendererAPI->init();
		}

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->setViewport(x, y, width, height);
		}

		inline static void setClearColor(const glm::vec4& color)
		{
			s_RendererAPI->setClearColor(color);
		}

		inline static void clear()
		{
			s_RendererAPI->clear();
		}

		inline static void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->drawIndexed(vertexArray, count);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}