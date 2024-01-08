#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Input/KeyCode.h"
#include "Muelsyse/Input/MouseCode.h"

namespace mul 
{
	class MUL_API Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		virtual ~Input() = default;
		
		inline static bool isKeyPressed(KeyCode key) { return s_Instance->isKeyPressedImpl(key); }

		inline static bool isMouseButtonPressed(MouseCode button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> getMousePosition() { return s_Instance->getMousePositionImpl(); }
		inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
		inline static float getMouseY() { return s_Instance->getMouseYImpl(); }

		static Scope<Input> create();
	protected:
		virtual bool isKeyPressedImpl(KeyCode key) = 0;

		virtual bool isMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
	private:
		static Scope<Input> s_Instance;
	};

}