#pragma once
#include "Muelsyse/Renderer/RendererAPI.h"

namespace mul
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void init() override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}