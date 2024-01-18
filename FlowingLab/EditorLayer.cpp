#include "EditorLayer.h"

#include <External/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Muelsyse/Scene/SceneSerializer.h>
#include <Muelsyse/Utils/Utils.h>
#include <ImGuizmo.h>
#include <Muelsyse/Math/Math.h>

using namespace mul;

EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_CameraController(1920.0f / 1080.0f)
{
	
}

void EditorLayer::onAttach()
{
	MUL_PROFILE_FUNCTION();

	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbSpec.width = 1920;
	fbSpec.height = 1080;
	m_Framebuffer = Framebuffer::create(fbSpec);

	m_ActiveScene = createRef<Scene>();

	auto commandLineArgs = Application::get().getCommandLineArgs();
	if (commandLineArgs.Count > 1)
	{
		auto sceneFilePath = commandLineArgs[1];
		SceneSerializer serializer(m_ActiveScene);
		serializer.deserialize(sceneFilePath);
	}

	m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
	// Entity
	auto square = m_ActiveScene->createEntity("Green Square");
	square.addComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

	auto redSquare = m_ActiveScene->createEntity("Red Square");
	redSquare.addComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	m_SquareEntity = square;
	m_CameraEntity = m_ActiveScene->createEntity("Camera A");
	m_CameraEntity.addComponent<CameraComponent>();

	m_SecondCamera = m_ActiveScene->createEntity("Camera B");
	auto &cc = m_SecondCamera.addComponent<CameraComponent>();
	cc.Primary = false;

	class CameraController : public ScriptableEntity
	{
	public:
		virtual void onCreate() override
		{
			auto &translation = getComponent<TransformComponent>().Translation;
			translation.x = rand() % 10 - 5.0f;
		}

		virtual void onDestroy() override
		{
		}

		virtual void onUpdate(Timestep ts) override
		{
			auto &translation = getComponent<TransformComponent>().Translation;
			float speed = 5.0f;

			if (Input::isKeyPressed(Key::A))
				translation.x -= speed * ts;
			if (Input::isKeyPressed(Key::D))
				translation.x += speed * ts;
			if (Input::isKeyPressed(Key::W))
				translation.y += speed * ts;
			if (Input::isKeyPressed(Key::S))
				translation.y -= speed * ts;
		}
	};

	m_CameraEntity.addComponent<NativeScriptComponent>().bind<CameraController>();
	m_SecondCamera.addComponent<NativeScriptComponent>().bind<CameraController>();
#endif

	m_SceneHierarchyPanel.setContext(m_ActiveScene);
}

void EditorLayer::onDetach()
{
	MUL_PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Timestep ts)
{
	MUL_PROFILE_FUNCTION();

	// Resize
	if (FramebufferSpecification spec = m_Framebuffer->getSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
	{
		m_Framebuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_CameraController.onResize(m_ViewportSize.x, m_ViewportSize.y);
		m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	}

	// Update
	if (m_ViewportFocused)
		m_CameraController.onUpdate(ts);

	m_EditorCamera.onUpdate(ts);

	// Render
	Renderer2D::resetStats();

	m_Framebuffer->bind();
	RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	RenderCommand::clear();

	// Clear our entity ID attachment to -1
	m_Framebuffer->clearAttachment(1, -1);
	
	m_ActiveScene->onUpdateEditor(ts, m_EditorCamera);

	auto[mx, my] = ImGui::GetMousePos();
	mx -= m_ViewportBounds[0].x;
	my -= m_ViewportBounds[0].y;
	glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
	my = viewportSize.y - my;
	int mouseX = (int)mx;
	int mouseY = (int)my;

	if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
	{
		int pixelData = m_Framebuffer->readPixel(1, mouseX, mouseY);
		m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
	}

	m_Framebuffer->unBind();
}

void EditorLayer::onImGuiRender()
{
	MUL_PROFILE_FUNCTION();

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport *viewport = ImGui::GetMainViewport();
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
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO &io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			// ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("New", "Ctrl+N"))
				newScene();

			if (ImGui::MenuItem("Open...", "Ctrl+O"))
				openScene();

			if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				saveSceneAs();

			if (ImGui::MenuItem("Exit"))
				Application::get().close();
			
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_SceneHierarchyPanel.onImGuiRender();

	ImGui::Begin("Stats");

	std::string name = "None";
	if (m_HoveredEntity)
		name = m_HoveredEntity.getComponent<TagComponent>().Tag;
	ImGui::Text("Hovered Entity: %s", name.c_str());
	
	auto stats = Renderer2D::getStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.getTotalVertexCount());
	ImGui::Text("Indices: %d", stats.getTotalIndexCount());

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	Application::get().getImGuiLayer()->blockEvents(!m_ViewportFocused && !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

	uint64_t textureID = m_Framebuffer->getColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

	// Gizmos
	Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
	if (selectedEntity && m_GizmoType != -1)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();

		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		// Runtime camera from entity
		// auto cameraEntity = m_ActiveScene->getPrimaryCameraEntity();
		// const auto& camera = cameraEntity.getComponent<CameraComponent>().Camera;
		// const glm::mat4& cameraProjection = camera.getProjection();
		// glm::mat4 cameraView = glm::inverse(cameraEntity.getComponent<TransformComponent>().getTransform());

		// Editor camera
		const glm::mat4& cameraProjection = m_EditorCamera.getProjection();
		glm::mat4 cameraView = m_EditorCamera.getViewMatrix();

		// Entity transform
		auto& tc = selectedEntity.getComponent<TransformComponent>();
		glm::mat4 transform = tc.getTransform();

		// Snapping
		bool snap = Input::isKeyPressed(Key::LeftControl);
		float snapValue = 0.5f; // Snap to 0.5m for translation/scale
		// Snap to 45 degrees for rotation
		if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
			snapValue = 45.0f;

		float snapValues[3] = { snapValue, snapValue, snapValue };

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
			(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
			nullptr, snap ? snapValues : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation, rotation, scale;
			Math::decomposeTransform(transform, translation, rotation, scale);

			glm::vec3 deltaRotation = rotation - tc.Rotation;
			tc.Translation = translation;
			tc.Rotation += deltaRotation;
			tc.Scale = scale;
		}
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void EditorLayer::onEvent(Event &e)
{
	m_CameraController.onEvent(e);
	m_EditorCamera.onEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.dispatcher<KeyPressedEvent>(MUL_BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
	dispatcher.dispatcher<MouseButtonPressedEvent>(MUL_BIND_EVENT_FUNC(EditorLayer::onMouseButtonPressed));
}

bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
{
	// Shortcuts
	if (e.getRepeatCount() > 0)
		return false;

	bool control = Input::isKeyPressed(Key::LeftControl) || Input::isKeyPressed(Key::RightControl);
	bool shift = Input::isKeyPressed(Key::LeftShift) || Input::isKeyPressed(Key::RightShift);

	switch (e.getKeyCode())
	{
		case Key::N:
		{
			if (control)
				newScene();
			break;
		}
		case Key::O:
		{
			if (control)
				openScene();
			break;
		}
		case Key::S:
		{
			if (control && shift)
				saveSceneAs();
			break;
		}

		// Gizmos
		case Key::Q:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}

	return true;
}

bool EditorLayer::onMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.getMouseButton() == Mouse::ButtonLeft)
	{
		if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::isKeyPressed(Key::LeftAlt))
			m_SceneHierarchyPanel.setSelectedEntity(m_HoveredEntity);
	}
	return false;
}

void EditorLayer::newScene()
{
	m_ActiveScene = createRef<Scene>();
	m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
	m_SceneHierarchyPanel.setContext(m_ActiveScene);
}

void EditorLayer::openScene()
{
	std::optional<std::string> filepath = FileDialogs::openFile("Muelsyse Scene (*.flowing)", "*.flowing");
	if (filepath)
	{
		m_ActiveScene = createRef<Scene>();
		m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.setContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
		serializer.deserialize(*filepath);
	}
}

void EditorLayer::saveSceneAs()
{
	std::optional<std::string> filepath = FileDialogs::saveFile("Muelsyse Scene (*.flowing)", "*.flowing");
	if (filepath)
	{
		SceneSerializer serializer(m_ActiveScene);
		serializer.serialize(*filepath);
	}
}