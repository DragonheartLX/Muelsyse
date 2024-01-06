#pragma once
#include "Muelsyse/Input/Input.h"

namespace mul 
{
	class WindowsInput: public Input
	{
	protected:
		virtual bool isKeyPressedImpl(KeyCode key) override;

		virtual bool isMouseButtonPressedImpl(MouseCode button) override;
		virtual std::pair<float, float> getMousePositionImpl() override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
	};

}