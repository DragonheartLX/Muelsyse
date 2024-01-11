#include "FlowingLab2D.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D(): 
	Layer("Sandbox2D"), 
	m_CameraController(1920.0f / 1080.0f, true), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{
}

void Sandbox2D::onAttach()
{
	MUL_PROFILE_FUNCTION();

	m_BgTexture = mul::Texture2D::create("assets/textures/picture.png");

	mul::FramebufferSpecification fbSpec;
	fbSpec.width = 1280;
	fbSpec.height = 720;
	m_Framebuffer = mul::Framebuffer::create(fbSpec);
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

	// Render
	mul::Renderer2D::resetStats();
	{
		MUL_PROFILE_SCOPE("Renderer Prep");
		m_Framebuffer->bind();
		mul::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		mul::RenderCommand::clear();
	}
	
	{
		static float rotation = 0.0f;
		rotation += ts * 50.0f;

		MUL_PROFILE_SCOPE("Renderer Draw");
		mul::Renderer2D::beginScene(m_CameraController.getCamera());
		mul::Renderer2D::drawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		mul::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		mul::Renderer2D::drawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, m_SquareColor);
		mul::Renderer2D::drawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_BgTexture, 10.0f);
		mul::Renderer2D::drawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_BgTexture, 20.0f);
		mul::Renderer2D::endScene();

		mul::Renderer2D::beginScene(m_CameraController.getCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				mul::Renderer2D::drawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		mul::Renderer2D::endScene();
		m_Framebuffer->unBind();
	}
}

void Sandbox2D::onImGuiRender()
{
	MUL_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) mul::Application::get().close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");

		auto stats = mul::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_Framebuffer->getColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1920, 1080 });
		ImGui::End();

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Settings");

		auto stats = mul::Renderer2D::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
		ImGui::Text("Indices: %d", stats.getTotalIndexCount());

		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		uint32_t textureID = m_BgTexture->getRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ 1920, 1080 });
		ImGui::End();
	}
}

void Sandbox2D::onEvent(mul::Event& e)
{
	m_CameraController.onEvent(e);
}