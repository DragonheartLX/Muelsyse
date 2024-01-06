#pragma once

namespace mul
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	} Mouse;
	
	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define MUL_MOUSE_BUTTON_0      ::mul::Mouse::Button0
#define MUL_MOUSE_BUTTON_1      ::mul::Mouse::Button1
#define MUL_MOUSE_BUTTON_2      ::mul::Mouse::Button2
#define MUL_MOUSE_BUTTON_3      ::mul::Mouse::Button3
#define MUL_MOUSE_BUTTON_4      ::mul::Mouse::Button4
#define MUL_MOUSE_BUTTON_5      ::mul::Mouse::Button5
#define MUL_MOUSE_BUTTON_6      ::mul::Mouse::Button6
#define MUL_MOUSE_BUTTON_7      ::mul::Mouse::Button7
#define MUL_MOUSE_BUTTON_LAST   ::mul::Mouse::ButtonLast
#define MUL_MOUSE_BUTTON_LEFT   ::mul::Mouse::ButtonLeft
#define MUL_MOUSE_BUTTON_RIGHT  ::mul::Mouse::ButtonRight
#define MUL_MOUSE_BUTTON_MIDDLE ::mul::Mouse::ButtonMiddle
