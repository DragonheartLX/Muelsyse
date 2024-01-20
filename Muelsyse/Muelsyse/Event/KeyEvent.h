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
		KeyPressedEvent(const KeyCode keycode, bool isRepeat = false): KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool isRepeat() const { return m_IsRepeat; }

		std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (repeat = " << m_IsRepeat << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		bool m_IsRepeat;
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