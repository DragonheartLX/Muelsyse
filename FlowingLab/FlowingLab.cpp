#include "FlowingLab.h"
// #include "ExampleLayer.h"
// #include "FlowingLab2D.h"

FlowingLab::FlowingLab():
	Application("FlowingLab Editor")
{
	// pushLayer(new ExampleLayer());
	// pushLayer(new Sandbox2D());
	pushLayer(new mul::EditorLayer());
};