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

	void newScene();
	void openScene();
	void saveSceneAs();

	mul::OrthographicCameraController m_CameraController;

	// Temp
	Ref<VertexArray> m_SquareVA;
	Ref<Shader> m_FlatColorShader;
	Ref<Framebuffer> m_Framebuffer;

	Ref<Scene> m_ActiveScene;
	Entity m_SquareEntity;
	Entity m_CameraEntity;
	Entity m_SecondCamera;

	EditorCamera m_EditorCamera;

	bool m_ViewportFocused = false, m_ViewportHovered = false;
	glm::vec2 m_ViewportSize = {0.0f, 0.0f};
	glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

	int m_GizmoType = -1;

	// Panels
	SceneHierarchyPanel m_SceneHierarchyPanel;
};