#pragma once
#include "mulpch.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/Input/KeyCode.h"

namespace mul 
{
	class MUL_API KeyEvent: public Event
	{
	public:
		KeyCode getKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		KeyEvent(const KeyCode keycode): m_KeyCode(keycode) {}

		KeyCode m_KeyCode;
	};

	class MUL_API KeyPressedEvent: public KeyEvent
	{
	public:
		KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount): KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		const uint16_t getRepeatCount() const { return m_RepeatCount; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t m_RepeatCount;
	};

	class MUL_API KeyReleasedEvent: public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode keycode): KeyEvent(keycode) {}

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
		KeyTypedEvent(const KeyCode keycode): KeyEvent(keycode) {}

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}