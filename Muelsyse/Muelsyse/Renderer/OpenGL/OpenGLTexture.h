#pragma once
#include "Muelsyse/Renderer/Texture.h"

namespace mul
{
	class MUL_API OpenGLTexture2D: public Texture2D
	{
	public:
		OpenGLTexture2D(const TextureSpecification& specification, Buffer data = Buffer());
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return m_Width;  }
		virtual uint32_t getHeight() const override { return m_Height; }
		virtual uint32_t getRendererID() const override { return m_RendererID; }

		virtual void setData(Buffer data) override;

		virtual void bind(uint32_t slot = 0) const override;
		
		virtual bool isLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == other.getRendererID();
		}
	private:
		TextureSpecification m_Specification;

		bool m_IsLoaded = false;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		unsigned int m_InternalFormat, m_DataFormat;
	};
}