#include "FlowingLab2D.h"

#include <External/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::onAttach()
{
	MUL_PROFILE_FUNCTION();

	m_BgTexture = mul::Texture2D::create("assets/textures/picture.png");
}

void Sandbox2D::onDetach()
{
	MUL_PROFILE_FUNCTION();
}

void Sandbox2D::onUpdate(mul::Timestep ts)
{
	MUL_PROFILE_FUNCTION();

	// Update
	m_CameraController.onUpdate(ts);

	{
		// Render
		MUL_PROFILE_SCOPE("Renderer Prep");
		mul::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		mul::RenderCommand::clear();
	}
	
	{
		MUL_PROFILE_SCOPE("Renderer Draw");
		mul::Renderer2D::beginScene(m_CameraController.getCamera());
		mul::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		mul::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		mul::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_BgTexture);
		mul::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender()
{
	MUL_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::onEvent(mul::Event& e)
{
	m_CameraController.onEvent(e);
}