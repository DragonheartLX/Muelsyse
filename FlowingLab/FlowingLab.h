#pragma once
#include <Muelsyse.h>
#include <Muelsyse/Core/EntryPoint.h>

#include "EditorLayer.h"

class FlowingLab: public mul::Application
{
public:
	FlowingLab(const mul::ApplicationSpecification& specification);
	~FlowingLab() {};

private:

};

FLOWING(FlowingLab, "../../");