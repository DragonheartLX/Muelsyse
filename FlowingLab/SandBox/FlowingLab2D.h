#pragma once
#include <Muelsyse.h>

class Sandbox2D: public mul::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void onAttach() override;
	virtual void onDetach() override;

	void onUpdate(mul::Timestep ts) override;
	virtual void onImGuiRender() override;
	void onEvent(mul::Event& e) override;
private:
	mul::OrthographicCameraController m_CameraController;
	
	// Temp
	mul::Ref<mul::VertexArray> m_SquareVA;
	mul::Ref<mul::Shader> m_FlatColorShader;
	mul::Ref<mul::Framebuffer> m_Framebuffer;
	mul::Ref<mul::Texture2D> m_BgTexture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};