#pragma once
#include "Muelsyse/Input/KeyCode.h"
#include "Muelsyse/Input/MouseCode.h"

#include <glm/glm.hpp>

namespace mul 
{
	class MUL_API Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}