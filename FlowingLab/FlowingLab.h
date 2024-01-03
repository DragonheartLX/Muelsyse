#pragma once
#include <Muelsyse.h>
#include <imgui.h>

class ExampleLayer: public mul::Layer
{
public:
	ExampleLayer();

	virtual void onUpdate() override;
	virtual void onImGuiRender() override;
	virtual void onEvent(mul::Event& event) override;
};

class FlowingLab : public mul::Application
{
public:
	FlowingLab();
	~FlowingLab() {};

private:

};

FLOWING(FlowingLab);