#pragma once

#define MUL_VERSION_STRING "0.0.0dev"

#include "Muelsyse/Core/PlatformDetection.h"

#ifdef MUL_DEBUG_ASSERT
	#if defined(MUL_PLATFORM_WINDOWS)
		#define MUL_DEBUGBREAK() __debugbreak()
	#elif defined(HZ_PLATFORM_LINUX)
		#include <signal.h>
		#define MUL_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define MUL_DEBUGBREAK()
#endif

#ifdef MUL_PLATFORM_WINDOWS
	#ifdef MUL_BUILD_SHARED
		#ifdef MUL_BUILD_DLL
			#define MUL_API __declspec(dllexport)
		#else
			#define MUL_API __declspec(dllimport)
		#endif
	#else
		#define MUL_API
	#endif
#else
	#error Muelsyse currently only support windows with MSVC
#endif

#define IMGUI_API MUL_API

#define MUL_EXPAND_MACRO(x) x
#define MUL_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define MUL_BIND_EVENT_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include <memory>

namespace mul
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> createScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#include "Muelsyse/Core/Assert.h"
