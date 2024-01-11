#pragma once
#include "Muelsyse/Core/Core.h"

namespace mul
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		// FramebufferFormat Format = 
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		
		virtual void bind() = 0;
		virtual void unBind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		static Ref<Framebuffer> MUL_API create(const FramebufferSpecification& spec);
	};
}