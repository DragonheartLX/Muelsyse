#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Input/KeyCode.h"
#include "Muelsyse/Input/MouseCode.h"

namespace mul 
{
	class MUL_API Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static std::pair<float, float> getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}