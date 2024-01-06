#include "mulpch.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLShader.h"
#include "Muelsyse/Renderer/Renderer2D.h"

namespace mul 
{
	Scope<Renderer::SceneData> Renderer::m_SceneData = createScope<Renderer::SceneData>();

	void Renderer::init()
	{
		MUL_PROFILE_FUNCTION();
		
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		Renderer2D::shutdown();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(OrthographicCamera& camera)
	{
		m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene()
	{

	}

	void Renderer::submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->bind();
		shader->setMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
		shader->setMat4("u_Transform", transform);

		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}

}