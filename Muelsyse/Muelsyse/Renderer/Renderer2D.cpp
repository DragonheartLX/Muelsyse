#include "mulpch.h"
#include "Muelsyse/Renderer/Renderer2D.h"
#include "Muelsyse/Renderer/VertexArray.h"
#include "Muelsyse/Renderer/Shader.h"
#include "Muelsyse/Renderer/RenderCommand.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>

namespace mul
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init()
	{
		MUL_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));

		squareVB->setLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data->QuadVertexArray->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));		
		s_Data->QuadVertexArray->setIndexBuffer(squareIB);

		s_Data->WhiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::create("assets/shaders/Texture.vert", "assets/shaders/Texture.frag");
		s_Data->TextureShader->bind();
		s_Data->TextureShader->setInt("u_Texture", 0);
	}

	void Renderer2D::shutdown()
	{
		MUL_PROFILE_FUNCTION();

		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera& camera)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->TextureShader->bind();
		s_Data->TextureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene()
	{
		MUL_PROFILE_FUNCTION();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		MUL_PROFILE_FUNCTION();

		s_Data->TextureShader->setFloat4("u_Color", color);
		s_Data->WhiteTexture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->setMat4("u_Transform", transform);

		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		drawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		MUL_PROFILE_FUNCTION();
		
		s_Data->TextureShader->setFloat4("u_Color", glm::vec4(1.0f));
		texture->bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->setMat4("u_Transform", transform);

		s_Data->QuadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->QuadVertexArray);
	}
}