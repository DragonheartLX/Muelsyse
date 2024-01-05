#pragma once

#ifdef MUL_PLATFORM_WINDOWS
	#if defined(_MSC_VER) && defined(MUL_BUILD_SHARED)
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

#ifdef MUL_DEBUG
	#define MUL_ASSERT(x, ...) { if(!(x)) { MUL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MUL_CORE_ASSERT(x, ...) { if(!(x)) { MUL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MUL_ASSERT(x, ...)
	#define MUL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MUL_BIND_EVENT_FUNC(fn) std::bind(&fn, this, std::placeholders::_1)

#include <memory>

namespace mul
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}