#include "mulpch.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLTexture.h"

#include "glad/glad.h"
#include <filesystem>

namespace mul 
{
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification, Buffer data):
		m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		MUL_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (data)
			setData(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		MUL_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(Buffer data)
	{
		MUL_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		MUL_CORE_ASSERT(data.Size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data.Data);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const
	{
		MUL_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}