#include "FlowingLab.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer():
	Layer("Example"), 
	m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), 
	m_CameraPosition(0.0f)
{
	m_VertexArray.reset(mul::VertexArray::create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		mul::Ref<mul::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(mul::VertexBuffer::create(vertices, sizeof(vertices)));
		mul::BufferLayout layout = {
			{ mul::ShaderDataType::Float3, "a_Position" },
			{ mul::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->setLayout(layout);
		m_VertexArray->addVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		mul::Ref<mul::IndexBuffer> indexBuffer;
		indexBuffer.reset(mul::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->setIndexBuffer(indexBuffer);

		m_SquareVA.reset(mul::VertexArray::create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		mul::Ref<mul::VertexBuffer> squareVB;
		squareVB.reset(mul::VertexBuffer::create(squareVertices, sizeof(squareVertices)));
		squareVB->setLayout({
			{ mul::ShaderDataType::Float3, "a_Position" },
			{ mul::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->addVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		mul::Ref<mul::IndexBuffer> squareIB;
		squareIB.reset(mul::IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->setIndexBuffer(squareIB);

		m_Shader.reset(mul::Shader::create("assets/shaders/Basic.vert", "assets/shaders/Basic.frag"));

		m_FlatColorShader.reset(mul::Shader::create("assets/shaders/FlatColor.vert", "assets/shaders/FlatColor.frag"));

		m_TextureShader.reset(mul::Shader::create("assets/shaders/Texture.vert", "assets/shaders/Texture.frag"));

		m_Texture = mul::Texture2D::create("assets/textures/picture.png");
		m_bgTexture = mul::Texture2D::create("assets/textures/bg.png");

		std::dynamic_pointer_cast<mul::OpenGLShader>(m_TextureShader)->bind();
		std::dynamic_pointer_cast<mul::OpenGLShader>(m_TextureShader)->setUniformInt("u_Texture", 0);
}

void ExampleLayer::onUpdate(mul::Timestep ts)
{
	if (mul::Input::isKeyPressed(MUL_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (mul::Input::isKeyPressed(MUL_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (mul::Input::isKeyPressed(MUL_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (mul::Input::isKeyPressed(MUL_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (mul::Input::isKeyPressed(MUL_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (mul::Input::isKeyPressed(MUL_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		mul::RenderCommand::setClearColor({ 0.164f, 0.164f, 0.164f, 1.0f });
		mul::RenderCommand::clear();

		m_Camera.setPosition(m_CameraPosition);
		m_Camera.setRotation(m_CameraRotation);

		mul::Renderer::beginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		std::dynamic_pointer_cast<mul::OpenGLShader>(m_FlatColorShader)->bind();
		std::dynamic_pointer_cast<mul::OpenGLShader>(m_FlatColorShader)->setUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				mul::Renderer::submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		
		m_bgTexture->bind();
		mul::Renderer::submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		
		m_Texture->bind();
		mul::Renderer::submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// mul::Renderer::submit(m_Shader, m_VertexArray);

		mul::Renderer::endScene();
}

void ExampleLayer::onImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::onEvent(mul::Event& event)
{
	
}

FlowingLab::FlowingLab()
{
	pushLayer(new ExampleLayer());
};