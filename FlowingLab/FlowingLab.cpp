#include "FlowingLab.h"

ExampleLayer::ExampleLayer():
	Layer("Example")
{

}

void ExampleLayer::onUpdate()
{
	if (mul::Input::isKeyPressed(MUL_KEY_TAB))
		MUL_TRACE("Tab key is pressed (poll)!");
}

void ExampleLayer::onImGuiRender()
{
	
}

void ExampleLayer::onEvent(mul::Event& event)
{
	if (event.getEventType() == mul::EventType::KeyPressed)
	{
		mul::KeyPressedEvent& e = (mul::KeyPressedEvent&)event;
		if (e.getKeyCode() == MUL_KEY_TAB)
			MUL_TRACE("Tab key is pressed (event)!");
		MUL_TRACE("{0}", (char)e.getKeyCode());
	}
}

FlowingLab::FlowingLab()
{
	pushLayer(new ExampleLayer());
};