#include "FlowingLab.h"

FlowingLab::FlowingLab(const mul::ApplicationSpecification& specification): 
	mul::Application(specification)
{
	pushLayer(new EditorLayer());
};