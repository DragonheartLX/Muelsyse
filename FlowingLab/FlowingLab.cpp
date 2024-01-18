#include "FlowingLab.h"
// #include "ExampleLayer.h"
// #include "FlowingLab2D.h"

FlowingLab::FlowingLab(ApplicationCommandLineArgs args):
	Application("FlowingLab Editor", args)
{
	// pushLayer(new ExampleLayer());
	// pushLayer(new Sandbox2D());
	pushLayer(new EditorLayer());
};