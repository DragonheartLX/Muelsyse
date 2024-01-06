#include "FlowingLab.h"
#include "ExampleLayer.h"
#include "FlowingLab2D.h"

FlowingLab::FlowingLab()
{
	// pushLayer(new ExampleLayer());
	pushLayer(new Sandbox2D());
};