#include "mulpch.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLVertexArray.h"

#include "External/glad/glad.h"

namespace mul
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case mul::ShaderDataType::Float:    return GL_FLOAT;
			case mul::ShaderDataType::Float2:   return GL_FLOAT;
			case mul::ShaderDataType::Float3:   return GL_FLOAT;
			case mul::ShaderDataType::Float4:   return GL_FLOAT;
			case mul::ShaderDataType::Mat3:     return GL_FLOAT;
			case mul::ShaderDataType::Mat4:     return GL_FLOAT;
			case mul::ShaderDataType::Int:      return GL_INT;
			case mul::ShaderDataType::Int2:     return GL_INT;
			case mul::ShaderDataType::Int3:     return GL_INT;
			case mul::ShaderDataType::Int4:     return GL_INT;
			case mul::ShaderDataType::Bool:     return GL_BOOL;
		}

		MUL_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::unBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		MUL_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->getLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)(intptr_t)element.offset);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->bind();
        
		m_IndexBuffer = indexBuffer;
	}
}