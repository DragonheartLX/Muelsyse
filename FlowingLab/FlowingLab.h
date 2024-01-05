#pragma once
#include <Muelsyse.h>

class ExampleLayer: public mul::Layer
{
public:
	ExampleLayer();

	virtual void onUpdate(mul::Timestep ts) override;
	virtual void onImGuiRender() override;
	virtual void onEvent(mul::Event& event) override;

	private:
	mul::Ref<mul::Shader> m_Shader;
	mul::Ref<mul::VertexArray> m_VertexArray;

	mul::Ref<mul::Shader> m_FlatColorShader, m_TextureShader;
	mul::Ref<mul::VertexArray> m_SquareVA;

	mul::Ref<mul::Texture2D> m_Texture, m_bgTexture;

	mul::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class FlowingLab : public mul::Application
{
public:
	FlowingLab();
	~FlowingLab() {};

private:

};

FLOWING(FlowingLab);