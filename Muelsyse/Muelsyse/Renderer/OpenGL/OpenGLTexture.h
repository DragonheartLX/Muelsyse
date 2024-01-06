#pragma once
#include "Muelsyse/Renderer/Texture.h"

namespace mul
{
	class MUL_API OpenGLTexture2D: public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t getWidth() const override { return m_Width;  }
		virtual uint32_t getHeight() const override { return m_Height; }

		virtual void setData(void* data, uint32_t size) override;

		virtual void bind(uint32_t slot = 0) const override;
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		unsigned int m_InternalFormat, m_DataFormat;
	};
}