#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Renderer/RenderCommand.h"
#include "Muelsyse/Renderer/OrthographicCamera.h"
#include "Muelsyse/Renderer/Shader.h"

namespace mul
{
	class MUL_API Renderer
	{
	public:
		static void init();

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}