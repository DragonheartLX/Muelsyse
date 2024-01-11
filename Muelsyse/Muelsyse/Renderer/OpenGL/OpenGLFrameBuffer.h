#pragma once
#include "Muelsyse/Renderer/FrameBuffer.h"

namespace mul
{
	class OpenGLFramebuffer: public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unBind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual uint32_t getColorAttachmentRendererID() const override { return m_ColorAttachment; }

		virtual const FramebufferSpecification& getSpecification() const override { return m_Specification; }
	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferSpecification m_Specification;
	};
}