#include "mulpch.h"
#include "Muelsyse/Input/Input.h"

#ifdef MUL_PLATFORM_WINDOWS
	#include "Muelsyse/Input/Windows/Input.h"
#endif

namespace mul
{

	Scope<Input> Input::s_Instance = Input::create();

	Scope<Input> Input::create()
	{
	#ifdef MUL_PLATFORM_WINDOWS
		return createScope<WindowsInput>();
	#else
		MUL_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
} 