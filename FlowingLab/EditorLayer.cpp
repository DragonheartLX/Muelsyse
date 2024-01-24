#include "EditorLayer.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Muelsyse/Scene/SceneSerializer.h>
#include <Muelsyse/Utils/Utils.h>
#include <Muelsyse/Script/ScriptEngine.h>
#include <ImGuizmo.h>
#include <Muelsyse/Math/Math.h>

using namespace mul;

EditorLayer::EditorLayer(): 
	Layer("EditorLayer")
{
	
}

void EditorLayer::onAttach()
{
	MUL_PROFILE_FUNCTION();

	m_IconPlay = Texture2D::create("Resources/Icons/PlayButton.png");
	m_IconPause = Texture2D::create("Resources/Icons/PauseButton.png");
	m_IconStep = Texture2D::create("Resources/Icons/StepButton.png");
	m_IconSimulate = Texture2D::create("Resources/Icons/SimulateButton.png");
	m_IconStop = Texture2D::create("Resources/Icons/StopButton.png");

	FramebufferSpecification fbSpec;
	fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
	fbSpec.width = 1920;
	fbSpec.height = 1080;
	m_Framebuffer = Framebuffer::create(fbSpec);

	m_EditorScene = createRef<Scene>();
	m_ActiveScene = m_EditorScene;
	m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

	auto commandLineArgs = Application::get().getSpecification().CommandLineArgs;
	if (commandLineArgs.Count > 1)
	{
		auto projectFilePath = commandLineArgs[1];
		openProject(projectFilePath);
	}
	else
	{
		// TODO: prompt the user to select a directory
		// newProject();

		// If no project is opened, close
		// NOTE: this is while we don't have a new project path
		if (!openProject())
			Application::get().close();
	}
	// Renderer2D::setLineWidth(1.0f);
}

void EditorLayer::onDetach()
{
	MUL_PROFILE_FUNCTION();
}

void EditorLayer::onUpdate(Timestep ts)
{
	MUL_PROFILE_FUNCTION();

	m_ActiveScene->onViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

	// Resize
	if (FramebufferSpecification spec = m_Framebuffer->getSpecification();
		m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
		(spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
	{
		m_Framebuffer->resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_EditorCamera.setViewportSize(m_ViewportSize.x, m_ViewportSize.y);
	}

	// Render
	Renderer2D::resetStats();

	m_Framebuffer->bind();
	RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1});
	RenderCommand::clear();

	// Clear our entity ID attachment to -1
	m_Framebuffer->clearAttachment(1, -1);
	
	switch (m_SceneState)
	{
		case SceneState::Edit:
		{
			m_EditorCamera.onUpdate(ts);

			m_ActiveScene->onUpdateEditor(ts, m_EditorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			m_EditorCamera.onUpdate(ts);

			m_ActiveScene->onUpdateSimulation(ts, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->onUpdateRuntime(ts);
			break;
		}
	}

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

	onOverlayRender();

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
			if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				openProject();

			ImGui::Separator();

			if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				newScene();

			if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				saveScene();

			if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				saveSceneAs();

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
				Application::get().close();
			
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Script"))
		{
			if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
				ScriptEngine::reloadAssembly();
			
			ImGui::EndMenu();
		}
		
		ImGui::EndMenuBar();
	}

	m_SceneHierarchyPanel.onImGuiRender();
	m_ContentBrowserPanel->onImGuiRender();

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
	ImGui::Text("FPS: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();

	ImGui::Begin("Settings");
	ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
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
	Application::get().getImGuiLayer()->blockEvents(!m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

	uint64_t textureID = m_Framebuffer->getColorAttachmentRendererID();
	ImGui::Image(reinterpret_cast<void *>(textureID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});

	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			openScene(path);
		}
		ImGui::EndDragDropTarget();
	}

	// Gizmos
	Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
	if (selectedEntity && m_GizmoType != -1 && m_SceneState != SceneState::Play)
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

	UI_Toolbar();

	ImGui::End();
}

void EditorLayer::UI_Toolbar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	auto& colors = ImGui::GetStyle().Colors;
	const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
	const auto& buttonActive = colors[ImGuiCol_ButtonActive];
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

	ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	bool toolbarEnabled = (bool)m_ActiveScene;

	ImVec4 tintColor = ImVec4(1, 1, 1, 1);
	if (!toolbarEnabled)
		tintColor.w = 0.5f;

	float size = ImGui::GetWindowHeight() - 4.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

	bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
	bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
	bool hasPauseButton = m_SceneState != SceneState::Edit;

	{
		Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererID()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
				onScenePlay();
			else if (m_SceneState == SceneState::Play)
				onSceneStop();
		}
	}

	if (hasSimulateButton)
	{
		if (hasPlayButton)
			ImGui::SameLine();

		Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererID()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
		{
			if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
				onSceneSimulate();
			else if (m_SceneState == SceneState::Simulate)
				onSceneStop();
		}
	}

	if (hasPauseButton)
	{
		bool isPaused = m_ActiveScene->isPaused();
		ImGui::SameLine();
		{
			Ref<Texture2D> icon = m_IconPause;
			if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererID()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				m_ActiveScene->setPaused(!isPaused);
			}
		}

		// Step button
		if (isPaused)
		{
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconStep;
				bool isPaused = m_ActiveScene->isPaused();
				if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->getRendererID()), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->step();
				}
			}
		}
	}

	ImGui::PopStyleVar(2);
	ImGui::PopStyleColor(3);
	ImGui::End();
}

void EditorLayer::onEvent(Event &e)
{
	if ((m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) && m_ViewportHovered)
		m_EditorCamera.onEvent(e);

	EventDispatcher dispatcher(e);
	dispatcher.dispatcher<KeyPressedEvent>(MUL_BIND_EVENT_FUNC(EditorLayer::onKeyPressed));
	dispatcher.dispatcher<MouseButtonPressedEvent>(MUL_BIND_EVENT_FUNC(EditorLayer::onMouseButtonPressed));
	dispatcher.dispatcher<WindowDropEvent>(MUL_BIND_EVENT_FUNC(EditorLayer::onWindowDrop));
}

bool EditorLayer::onKeyPressed(KeyPressedEvent& e)
{
	// Shortcuts
	if (e.isRepeat())
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
				openProject();
			break;
		}
		case Key::S:
		{
			if (control)
			{
				if (shift)
					saveSceneAs();
				else
					saveScene();
			}

			break;
		}

		// Scene Commands
		case Key::D:
		{
			if (control)
				onDuplicateEntity();

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
			if (control)
				{
					ScriptEngine::reloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						m_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
			break;
		}
		case Key::Delete:
		{
			if (Application::get().getImGuiLayer()->getActiveWidgetID() == 0)
			{
				Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
				if (selectedEntity)
				{
					m_SceneHierarchyPanel.setSelectedEntity({});
					m_ActiveScene->destroyEntity(selectedEntity);
				}
			}
			break;
		}
	}

	return false;
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

bool EditorLayer::onWindowDrop(WindowDropEvent& e)
{
	// TODO: if a project is dropped in, probably open it

	//AssetManager::importAsset();

	return true;
}

void EditorLayer::onOverlayRender()
{
	if (m_SceneState == SceneState::Play)
	{
		Entity camera = m_ActiveScene->getPrimaryCameraEntity();
		if (!camera)
			return;
		
		Renderer2D::beginScene(camera.getComponent<CameraComponent>().Camera, camera.getComponent<TransformComponent>().getTransform(camera.getComponent<CameraComponent>().FixedRotation));
	}
	else
	{
		Renderer2D::beginScene(m_EditorCamera);
	}

	if (m_ShowPhysicsColliders)
	{
		// Box Colliders
		{
			auto view = m_ActiveScene->getAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
			for (auto entity : view)
			{
				auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

				glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
				glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
					* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
					* glm::scale(glm::mat4(1.0f), scale);

				Renderer2D::drawRect(transform, glm::vec4(0, 1, 0, 1));
			}
		}

		// Circle Colliders
		{
			auto view = m_ActiveScene->getAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
			for (auto entity : view)
			{
				auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

				glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
				glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
					* glm::scale(glm::mat4(1.0f), scale);

				Renderer2D::drawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
			}
		}
	}

	// Draw selected entity outline 
	Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
	if (selectedEntity && m_SceneState != SceneState::Play)
	{
		const TransformComponent& transform = selectedEntity.getComponent<TransformComponent>();
		Renderer2D::drawRect(transform.getTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
	}

	Renderer2D::endScene();
}

void EditorLayer::newProject()
{
	Project::newProject();
}

void EditorLayer::openProject(const std::filesystem::path& path)
{
	if (Project::loadProject(path))
	{
		auto startScenePath = Project::getAssetFileSystemPath(Project::getActive()->getConfig().StartScene);
		openScene(startScenePath);
		m_ContentBrowserPanel = createScope<ContentBrowserPanel>();
	}
}

bool EditorLayer::openProject()
{
	std::string filepath = FileDialogs::openFile("Muelsyse Project (*.mproj)", "*.mproj");
	if (filepath.empty())
		return false;

	openProject(filepath);
	return true;
}

void EditorLayer::saveProject()
{
	// Project::SaveActive();
}

void EditorLayer::newScene()
{
	m_ActiveScene = createRef<Scene>();

	m_SceneHierarchyPanel.setContext(m_ActiveScene);

	m_EditorScenePath = std::filesystem::path();
}

void EditorLayer::openScene()
{	
	std::string filepath = FileDialogs::openFile("Muelsyse Scene (*.flowing)", "*.flowing");
	if (!filepath.empty())
		openScene(filepath);
}

void EditorLayer::openScene(const std::filesystem::path& path)
{
	if (m_SceneState != SceneState::Edit)
			onSceneStop();
	
	if (path.extension().string() != ".flowing")
	{
		MUL_WARN("Could not load {0} - not a scene file", path.filename().string());
		return;
	}
		
	Ref<Scene> newScene = createRef<Scene>();
	SceneSerializer serializer(newScene);
	if (serializer.deserialize(path.string()))
	{
		m_EditorScene = newScene;
		
		m_SceneHierarchyPanel.setContext(m_EditorScene);

		m_ActiveScene = m_EditorScene;
		m_EditorScenePath = path;
	}
}

void EditorLayer::saveScene()
{
	if (!m_EditorScenePath.empty())
		serializeScene(m_ActiveScene, m_EditorScenePath);
	else
		saveSceneAs();
}

void EditorLayer::saveSceneAs()
{
	std::string filepath = FileDialogs::saveFile("Muelsyse Scene (*.flowing)", "*.flowing");
	if (!filepath.empty())
	{
		serializeScene(m_ActiveScene, filepath);
		m_EditorScenePath = filepath;
	}
}

void EditorLayer::serializeScene(Ref<Scene> scene, const std::filesystem::path& path)
{
	SceneSerializer serializer(scene);
	serializer.serialize(path.string());
}

void EditorLayer::onScenePlay()
{
	if (m_SceneState == SceneState::Simulate)
		onSceneStop();
	
	m_SceneState = SceneState::Play;

	m_ActiveScene = Scene::copy(m_EditorScene);
	m_ActiveScene->onRuntimeStart();

	m_SceneHierarchyPanel.setContext(m_ActiveScene);
}

void EditorLayer::onSceneSimulate()
{
	if (m_SceneState == SceneState::Play)
		onSceneStop();

	m_SceneState = SceneState::Simulate;

	m_ActiveScene = Scene::copy(m_EditorScene);
	m_ActiveScene->onSimulationStart();

	m_SceneHierarchyPanel.setContext(m_ActiveScene);
}

void EditorLayer::onSceneStop()
{
	MUL_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);

	if (m_SceneState == SceneState::Play)
		m_ActiveScene->onRuntimeStop();
	else if (m_SceneState == SceneState::Simulate)
		m_ActiveScene->onSimulationStop();
	
	m_SceneState = SceneState::Edit;

	m_ActiveScene = m_EditorScene;

	m_SceneHierarchyPanel.setContext(m_ActiveScene);
}

void EditorLayer::onScenePause()
{
	if (m_SceneState == SceneState::Edit)
		return;

	m_ActiveScene->setPaused(true);
}

void EditorLayer::onDuplicateEntity()
{
	if (m_SceneState != SceneState::Edit)
		return;

	Entity selectedEntity = m_SceneHierarchyPanel.getSelectedEntity();
	if (selectedEntity)
	{
		Entity newEntity = m_EditorScene->duplicateEntity(selectedEntity);
		m_SceneHierarchyPanel.setSelectedEntity(newEntity);
	}
}