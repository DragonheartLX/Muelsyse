#pragma once
#include "Muelsyse/Renderer/Buffer.h"

namespace mul
{
	class OpenGLVertexBuffer: public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual void setData(const void* data, uint32_t size) override;

		virtual const BufferLayout& getLayout() const override { return m_Layout; }
		virtual void setLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer: public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const;
		virtual void unBind() const;

		virtual uint32_t getCount() const { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}