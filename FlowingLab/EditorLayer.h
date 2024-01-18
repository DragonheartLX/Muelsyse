#pragma once
#include <Muelsyse.h>

#include "Panels/SceneHierarchyPanel.h"

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

	void newScene();
	void openScene();
	void saveSceneAs();

	mul::OrthographicCameraController m_CameraController;
	Ref<Framebuffer> m_Framebuffer;
	Ref<Scene> m_ActiveScene;

	Entity m_HoveredEntity;
	
	EditorCamera m_EditorCamera;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = {0.0f, 0.0f};
	glm::vec2 m_ViewportBounds[2];

	int m_GizmoType = -1;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
};