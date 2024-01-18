#pragma once
#include <Muelsyse.h>
#include <Muelsyse/Core/EntryPoint.h>

#include "EditorLayer.h"

class FlowingLab: public mul::Application
{
public:
	FlowingLab(ApplicationCommandLineArgs args);
	~FlowingLab() {};

private:

};

FLOWING(FlowingLab);