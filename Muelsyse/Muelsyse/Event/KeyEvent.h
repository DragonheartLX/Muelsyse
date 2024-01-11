#pragma once
#include "mulpch.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/Input/Input.h"

namespace mul 
{
	class MUL_API KeyEvent: public Event
	{
	public:
		KeyCode getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(KeyCode keycode): m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class MUL_API KeyPressedEvent: public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount): KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		int getRepeatCount() const { return m_RepeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};

	class MUL_API KeyReleasedEvent: public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode): KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class MUL_API KeyTypedEvent: public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode): KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}