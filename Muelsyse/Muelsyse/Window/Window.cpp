#include "mulpch.h"
#include "Muelsyse/Window/Window.h"

#ifdef MUL_PLATFORM_WINDOWS
	#include "Muelsyse/Window/Windows/Window.h"
#endif

namespace mul
{
	Scope<Window> Window::create(const WindowProps& props)
	{
	#ifdef MUL_PLATFORM_WINDOWS
		return createScope<WindowsWindow>(props);
	#else
		MUL_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}