#include "mulpch.h"
#include "Muelsyse/Renderer/Renderer2D.h"
#include "Muelsyse/Renderer/VertexArray.h"
#include "Muelsyse/Renderer/Shader.h"
#include "Muelsyse/Renderer/RenderCommand.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mul
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::init()
	{
		MUL_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = VertexArray::create();

		s_Data->QuadVertexBuffer = VertexBuffer::create(s_Data->MaxVertices * sizeof(QuadVertex));

		s_Data->QuadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		});
		s_Data->QuadVertexArray->addVertexBuffer(s_Data->QuadVertexBuffer);

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::create(quadIndices, s_Data->MaxIndices);
		s_Data->QuadVertexArray->setIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data->WhiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++)
			samplers[i] = i;
		
		s_Data->TextureShader = Shader::create("assets/shaders/Texture.vert", "assets/shaders/Texture.frag");
		s_Data->TextureShader->bind();
		s_Data->TextureShader->setIntArray("u_Textures", samplers, s_Data->MaxTextureSlots);

		// Set all texture slots to 0
		s_Data->TextureSlots[0] = s_Data->WhiteTexture;

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::shutdown()
	{
		MUL_PROFILE_FUNCTION();

		delete[] s_Data->QuadVertexBufferBase;
		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->TextureShader->bind();
		s_Data->TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::endScene()
	{
		MUL_PROFILE_FUNCTION();

		uint32_t dataSize = (uint32_t)( (uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase );
		s_Data->QuadVertexBuffer->setData(s_Data->QuadVertexBufferBase, dataSize);

		flush();
	}

	void Renderer2D::flush()
	{
		if (s_Data->QuadIndexCount == 0)
			return; // Nothing to draw
		
		// Bind textures
		for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
			s_Data->TextureSlots[i]->bind(i);
		
		RenderCommand::drawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
		s_Data->Stats.DrawCalls++;
	}

	void Renderer2D::m_flushAndReset()
	{
		endScene();

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MUL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

			for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MUL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_flushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++)
		{
			if (*s_Data->TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				m_flushAndReset();

			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = tintColor;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		MUL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_flushAndReset();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MUL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_flushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data->TextureSlotIndex; i++)
		{
			if (*s_Data->TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data->TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
				m_flushAndReset();
			
			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = tintColor;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;
	
		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::resetStats()
	{
		memset(&s_Data->Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::getStats()
	{
		return s_Data->Stats;
	}
}