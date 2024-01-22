#pragma once
#include <Muelsyse.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

using namespace mul;

class EditorLayer : public Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(Event &e) override;

private:
	bool onKeyPressed(KeyPressedEvent& e);
	bool onMouseButtonPressed(MouseButtonPressedEvent& e);
	bool onWindowDrop(WindowDropEvent& e);

	void onOverlayRender();

	void newProject();
	bool openProject();
	void openProject(const std::filesystem::path& path);
	void saveProject();

	void newScene();
	void openScene();
	void openScene(const std::filesystem::path& path);
	void saveScene();
	void saveSceneAs();

	void serializeScene(Ref<Scene> scene, const std::filesystem::path& path);

	void onScenePlay();
	void onSceneSimulate();
	void onSceneStop();

	void onDuplicateEntity();


	// UI Panels
	void UI_Toolbar();

	Ref<Framebuffer> m_Framebuffer;
	Ref<Scene> m_ActiveScene;
	Ref<Scene> m_EditorScene;
	std::filesystem::path m_EditorScenePath;

	Entity m_HoveredEntity;
	
	EditorCamera m_EditorCamera;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = {1920.0f, 1080.0f};
	glm::vec2 m_ViewportBounds[2];

	int m_GizmoType = -1;

	bool m_ShowPhysicsColliders = false;

	enum class SceneState
	{
		Edit = 0, Play = 1, Simulate = 2
	};
	SceneState m_SceneState = SceneState::Edit;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
	Scope<ContentBrowserPanel> m_ContentBrowserPanel;

	// Editor resources
	Ref<Texture2D> m_IconPlay, m_IconSimulate, m_IconStop;
};