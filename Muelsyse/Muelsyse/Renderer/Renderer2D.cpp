#include "mulpch.h"
#include "Muelsyse/Renderer/Renderer2D.h"
#include "Muelsyse/Renderer/VertexArray.h"
#include "Muelsyse/Renderer/Shader.h"
#include "Muelsyse/Renderer/RenderCommand.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"
#include "Muelsyse/Renderer/UniformBuffer.h"
#include "Muelsyse/Asset/Assetmanager.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace mul
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
		// Editor-only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};	

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> WhiteTexture;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 1.0f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};

	static Renderer2DData* s_Data;

	void Renderer2D::init()
	{
		MUL_PROFILE_FUNCTION();

		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = VertexArray::create();

		s_Data->QuadVertexBuffer = VertexBuffer::create(s_Data->MaxVertices * sizeof(QuadVertex));

		s_Data->QuadVertexBuffer->setLayout({
			{ ShaderDataType::Float3, 	"a_Position" 	},
			{ ShaderDataType::Float4, 	"a_Color" 		},
			{ ShaderDataType::Float2, 	"a_TexCoord" 	},
			{ ShaderDataType::Float, 	"a_TexIndex" 	},
			{ ShaderDataType::Float, 	"a_TilingFactor"},
			{ ShaderDataType::Int,    	"a_EntityID"    }
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

		// Circles
		s_Data->CircleVertexArray = VertexArray::create();

		s_Data->CircleVertexBuffer = VertexBuffer::create(s_Data->MaxVertices * sizeof(CircleVertex));
		s_Data->CircleVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
		});
		s_Data->CircleVertexArray->addVertexBuffer(s_Data->CircleVertexBuffer);
		s_Data->CircleVertexArray->setIndexBuffer(quadIB); // Use quad IB
		s_Data->CircleVertexBufferBase = new CircleVertex[s_Data->MaxVertices];

		// Lines
		s_Data->LineVertexArray = VertexArray::create();

		s_Data->LineVertexBuffer = VertexBuffer::create(s_Data->MaxVertices * sizeof(LineVertex));
		s_Data->LineVertexBuffer->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
		});
		s_Data->LineVertexArray->addVertexBuffer(s_Data->LineVertexBuffer);
		s_Data->LineVertexBufferBase = new LineVertex[s_Data->MaxVertices];

		s_Data->WhiteTexture = Texture2D::create(TextureSpecification());
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->setData(Buffer(&whiteTextureData, sizeof(uint32_t)));

		int32_t samplers[s_Data->MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data->MaxTextureSlots; i++)
			samplers[i] = i;
		
		s_Data->QuadShader = Shader::create("Resources/Shaders/Renderer2D_Quad.glsl");
		s_Data->CircleShader = Shader::create("Resources/Shaders/Renderer2D_Circle.glsl");
		s_Data->LineShader = Shader::create("Resources/Shaders/Renderer2D_Line.glsl");

		// Set first texture slot to 0
		s_Data->TextureSlots[0] = s_Data->WhiteTexture;

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		s_Data->CameraUniformBuffer = UniformBuffer::create(sizeof(Renderer2DData::CameraData), 0);
	}

	void Renderer2D::shutdown()
	{
		MUL_PROFILE_FUNCTION();

		delete[] s_Data->QuadVertexBufferBase;
		delete s_Data;
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = camera.getProjection() * glm::inverse(transform);
		s_Data->CameraUniformBuffer->setData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		m_StartBatch();
	}

	void Renderer2D::beginScene(const EditorCamera& camera)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = camera.getViewProjection();
		s_Data->CameraUniformBuffer->setData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		m_StartBatch();
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->CameraBuffer.ViewProjection = camera.getProjectionMatrix();
		s_Data->CameraUniformBuffer->setData(&s_Data->CameraBuffer, sizeof(Renderer2DData::CameraData));

		m_StartBatch();
	}

	void Renderer2D::endScene()
	{
		MUL_PROFILE_FUNCTION();

		flush();
	}

	void Renderer2D::m_StartBatch()
	{
		s_Data->QuadIndexCount = 0;	
		s_Data->QuadVertexBufferPtr = s_Data->QuadVertexBufferBase;

		s_Data->CircleIndexCount = 0;
		s_Data->CircleVertexBufferPtr = s_Data->CircleVertexBufferBase;

		s_Data->LineVertexCount = 0;
		s_Data->LineVertexBufferPtr = s_Data->LineVertexBufferBase;

		s_Data->TextureSlotIndex = 1;
	}

	void Renderer2D::flush()
	{
		if (s_Data->QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->QuadVertexBufferPtr - (uint8_t*)s_Data->QuadVertexBufferBase);
			s_Data->QuadVertexBuffer->setData(s_Data->QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < s_Data->TextureSlotIndex; i++)
				s_Data->TextureSlots[i]->bind(i);

			s_Data->QuadShader->bind();
			RenderCommand::drawIndexed(s_Data->QuadVertexArray, s_Data->QuadIndexCount);
			s_Data->Stats.DrawCalls++;
		}

		if (s_Data->CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->CircleVertexBufferPtr - (uint8_t*)s_Data->CircleVertexBufferBase);
			s_Data->CircleVertexBuffer->setData(s_Data->CircleVertexBufferBase, dataSize);

			s_Data->CircleShader->bind();
			RenderCommand::drawIndexed(s_Data->CircleVertexArray, s_Data->CircleIndexCount);
			s_Data->Stats.DrawCalls++;
		}

		if (s_Data->LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data->LineVertexBufferPtr - (uint8_t*)s_Data->LineVertexBufferBase);
			s_Data->LineVertexBuffer->setData(s_Data->LineVertexBufferBase, dataSize);

			s_Data->LineShader->bind();
			RenderCommand::setLineWidth(s_Data->LineWidth);
			RenderCommand::drawLines(s_Data->LineVertexArray, s_Data->LineVertexCount);
			s_Data->Stats.DrawCalls++;
		}
	}

	void Renderer2D::m_NextBatch()
	{
		flush();
		m_StartBatch();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MUL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		
		drawQuad(transform, color);
	}
	
	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MUL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, tilingFactor);
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		MUL_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float tilingFactor = 1.0f;

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_NextBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = color;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr->EntityID = entityID;
			s_Data->QuadVertexBufferPtr++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::drawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, int entityID)
	{
		MUL_PROFILE_FUNCTION();
		MUL_CORE_VERIFY(texture);

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
			m_NextBatch();

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
				m_NextBatch();

			textureIndex = (float)s_Data->TextureSlotIndex;
			s_Data->TextureSlots[s_Data->TextureSlotIndex] = texture;
			s_Data->TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPtr->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPtr->Color = tintColor;
			s_Data->QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data->QuadVertexBufferPtr->EntityID = entityID;
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

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, color);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		drawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		MUL_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		drawQuad(transform, texture, tilingFactor, tintColor);
	}

	void Renderer2D::drawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness /*= 1.0f*/, float fade /*= 0.005f*/, int entityID /*= -1*/)
	{
		MUL_PROFILE_FUNCTION();

		// TODO: implement for circles
		// if (s_Data->QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data->CircleVertexBufferPtr->WorldPosition = transform * s_Data->QuadVertexPositions[i];
			s_Data->CircleVertexBufferPtr->LocalPosition = s_Data->QuadVertexPositions[i] * 2.0f;
			s_Data->CircleVertexBufferPtr->Color = color;
			s_Data->CircleVertexBufferPtr->Thickness = thickness;
			s_Data->CircleVertexBufferPtr->Fade = fade;
			s_Data->CircleVertexBufferPtr->EntityID = entityID;
			s_Data->CircleVertexBufferPtr++;
		}

		s_Data->CircleIndexCount += 6;

		s_Data->Stats.QuadCount++;
	}

	void Renderer2D::drawLine(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data->LineVertexBufferPtr->Position = p0;
		s_Data->LineVertexBufferPtr->Color = color;
		s_Data->LineVertexBufferPtr->EntityID = entityID;
		s_Data->LineVertexBufferPtr++;

		s_Data->LineVertexBufferPtr->Position = p1;
		s_Data->LineVertexBufferPtr->Color = color;
		s_Data->LineVertexBufferPtr->EntityID = entityID;
		s_Data->LineVertexBufferPtr++;

		s_Data->LineVertexCount += 2;
	}

	void Renderer2D::drawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int entityID)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		drawLine(p0, p1, color, entityID);
		drawLine(p1, p2, color, entityID);
		drawLine(p2, p3, color, entityID);
		drawLine(p3, p0, color, entityID);
	}

	void Renderer2D::drawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data->QuadVertexPositions[i];

		drawLine(lineVertices[0], lineVertices[1], color, entityID);
		drawLine(lineVertices[1], lineVertices[2], color, entityID);
		drawLine(lineVertices[2], lineVertices[3], color, entityID);
		drawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::drawSprite(const glm::mat4& transform, SpriteRendererComponent& src, int entityID)
	{
		if (src.Texture)
		{
			Ref<Texture2D> texture = AssetManager::getAsset<Texture2D>(src.Texture);
			drawQuad(transform, texture, src.TilingFactor, src.Color, entityID);
		}
		else
		{
			drawQuad(transform, src.Color, entityID);
		}
	}

	float Renderer2D::getLineWidth()
	{
		return s_Data->LineWidth;
	}

	void Renderer2D::setLineWidth(float width)
	{
		s_Data->LineWidth = width;
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