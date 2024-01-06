#pragma once

namespace mul
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define MUL_KEY_SPACE           ::mul::Key::Space
#define MUL_KEY_APOSTROPHE      ::mul::Key::Apostrophe    /* ' */
#define MUL_KEY_COMMA           ::mul::Key::Comma         /* , */
#define MUL_KEY_MINUS           ::mul::Key::Minus         /* - */
#define MUL_KEY_PERIOD          ::mul::Key::Period        /* . */
#define MUL_KEY_SLASH           ::mul::Key::Slash         /* / */
#define MUL_KEY_0               ::mul::Key::D0
#define MUL_KEY_1               ::mul::Key::D1
#define MUL_KEY_2               ::mul::Key::D2
#define MUL_KEY_3               ::mul::Key::D3
#define MUL_KEY_4               ::mul::Key::D4
#define MUL_KEY_5               ::mul::Key::D5
#define MUL_KEY_6               ::mul::Key::D6
#define MUL_KEY_7               ::mul::Key::D7
#define MUL_KEY_8               ::mul::Key::D8
#define MUL_KEY_9               ::mul::Key::D9
#define MUL_KEY_SEMICOLON       ::mul::Key::Semicolon     /* ; */
#define MUL_KEY_EQUAL           ::mul::Key::Equal         /* = */
#define MUL_KEY_A               ::mul::Key::A
#define MUL_KEY_B               ::mul::Key::B
#define MUL_KEY_C               ::mul::Key::C
#define MUL_KEY_D               ::mul::Key::D
#define MUL_KEY_E               ::mul::Key::E
#define MUL_KEY_F               ::mul::Key::F
#define MUL_KEY_G               ::mul::Key::G
#define MUL_KEY_H               ::mul::Key::H
#define MUL_KEY_I               ::mul::Key::I
#define MUL_KEY_J               ::mul::Key::J
#define MUL_KEY_K               ::mul::Key::K
#define MUL_KEY_L               ::mul::Key::L
#define MUL_KEY_M               ::mul::Key::M
#define MUL_KEY_N               ::mul::Key::N
#define MUL_KEY_O               ::mul::Key::O
#define MUL_KEY_P               ::mul::Key::P
#define MUL_KEY_Q               ::mul::Key::Q
#define MUL_KEY_R               ::mul::Key::R
#define MUL_KEY_S               ::mul::Key::S
#define MUL_KEY_T               ::mul::Key::T
#define MUL_KEY_U               ::mul::Key::U
#define MUL_KEY_V               ::mul::Key::V
#define MUL_KEY_W               ::mul::Key::W
#define MUL_KEY_X               ::mul::Key::X
#define MUL_KEY_Y               ::mul::Key::Y
#define MUL_KEY_Z               ::mul::Key::Z
#define MUL_KEY_LEFT_BRACKET    ::mul::Key::LeftBracket   /* [ */
#define MUL_KEY_BACKSLASH       ::mul::Key::Backslash     /* \ */
#define MUL_KEY_RIGHT_BRACKET   ::mul::Key::RightBracket  /* ] */
#define MUL_KEY_GRAVE_ACCENT    ::mul::Key::GraveAccent   /* ` */
#define MUL_KEY_WORLD_1         ::mul::Key::World1        /* non-US #1 */
#define MUL_KEY_WORLD_2         ::mul::Key::World2        /* non-US #2 */

/* Function keys */
#define MUL_KEY_ESCAPE          ::mul::Key::Escape
#define MUL_KEY_ENTER           ::mul::Key::Enter
#define MUL_KEY_TAB             ::mul::Key::Tab
#define MUL_KEY_BACKSPACE       ::mul::Key::Backspace
#define MUL_KEY_INSERT          ::mul::Key::Insert
#define MUL_KEY_DELETE          ::mul::Key::Delete
#define MUL_KEY_RIGHT           ::mul::Key::Right
#define MUL_KEY_LEFT            ::mul::Key::Left
#define MUL_KEY_DOWN            ::mul::Key::Down
#define MUL_KEY_UP              ::mul::Key::Up
#define MUL_KEY_PAGE_UP         ::mul::Key::PageUp
#define MUL_KEY_PAGE_DOWN       ::mul::Key::PageDown
#define MUL_KEY_HOME            ::mul::Key::Home
#define MUL_KEY_END             ::mul::Key::End
#define MUL_KEY_CAPS_LOCK       ::mul::Key::CapsLock
#define MUL_KEY_SCROLL_LOCK     ::mul::Key::ScrollLock
#define MUL_KEY_NUM_LOCK        ::mul::Key::NumLock
#define MUL_KEY_PRINT_SCREEN    ::mul::Key::PrintScreen
#define MUL_KEY_PAUSE           ::mul::Key::Pause
#define MUL_KEY_F1              ::mul::Key::F1
#define MUL_KEY_F2              ::mul::Key::F2
#define MUL_KEY_F3              ::mul::Key::F3
#define MUL_KEY_F4              ::mul::Key::F4
#define MUL_KEY_F5              ::mul::Key::F5
#define MUL_KEY_F6              ::mul::Key::F6
#define MUL_KEY_F7              ::mul::Key::F7
#define MUL_KEY_F8              ::mul::Key::F8
#define MUL_KEY_F9              ::mul::Key::F9
#define MUL_KEY_F10             ::mul::Key::F10
#define MUL_KEY_F11             ::mul::Key::F11
#define MUL_KEY_F12             ::mul::Key::F12
#define MUL_KEY_F13             ::mul::Key::F13
#define MUL_KEY_F14             ::mul::Key::F14
#define MUL_KEY_F15             ::mul::Key::F15
#define MUL_KEY_F16             ::mul::Key::F16
#define MUL_KEY_F17             ::mul::Key::F17
#define MUL_KEY_F18             ::mul::Key::F18
#define MUL_KEY_F19             ::mul::Key::F19
#define MUL_KEY_F20             ::mul::Key::F20
#define MUL_KEY_F21             ::mul::Key::F21
#define MUL_KEY_F22             ::mul::Key::F22
#define MUL_KEY_F23             ::mul::Key::F23
#define MUL_KEY_F24             ::mul::Key::F24
#define MUL_KEY_F25             ::mul::Key::F25

/* Keypad */
#define MUL_KEY_KP_0            ::mul::Key::KP0
#define MUL_KEY_KP_1            ::mul::Key::KP1
#define MUL_KEY_KP_2            ::mul::Key::KP2
#define MUL_KEY_KP_3            ::mul::Key::KP3
#define MUL_KEY_KP_4            ::mul::Key::KP4
#define MUL_KEY_KP_5            ::mul::Key::KP5
#define MUL_KEY_KP_6            ::mul::Key::KP6
#define MUL_KEY_KP_7            ::mul::Key::KP7
#define MUL_KEY_KP_8            ::mul::Key::KP8
#define MUL_KEY_KP_9            ::mul::Key::KP9
#define MUL_KEY_KP_DECIMAL      ::mul::Key::KPDecimal
#define MUL_KEY_KP_DIVIDE       ::mul::Key::KPDivide
#define MUL_KEY_KP_MULTIPLY     ::mul::Key::KPMultiply
#define MUL_KEY_KP_SUBTRACT     ::mul::Key::KPSubtract
#define MUL_KEY_KP_ADD          ::mul::Key::KPAdd
#define MUL_KEY_KP_ENTER        ::mul::Key::KPEnter
#define MUL_KEY_KP_EQUAL        ::mul::Key::KPEqual

#define MUL_KEY_LEFT_SHIFT      ::mul::Key::LeftShift
#define MUL_KEY_LEFT_CONTROL    ::mul::Key::LeftControl
#define MUL_KEY_LEFT_ALT        ::mul::Key::LeftAlt
#define MUL_KEY_LEFT_SUPER      ::mul::Key::LeftSuper
#define MUL_KEY_RIGHT_SHIFT     ::mul::Key::RightShift
#define MUL_KEY_RIGHT_CONTROL   ::mul::Key::RightControl
#define MUL_KEY_RIGHT_ALT       ::mul::Key::RightAlt
#define MUL_KEY_RIGHT_SUPER     ::mul::Key::RightSuper
#define MUL_KEY_MENU            ::mul::Key::Menu
