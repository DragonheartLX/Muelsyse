#pragma once

#define MUL_VERSION_STRING "0.0.0dev"

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows x64/x86 */
	#ifdef _WIN64
		/* Windows x64  */
		#define MUL_PLATFORM_WINDOWS
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define MUL_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define MUL_PLATFORM_MACOS
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform!"
	#endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#define MUL_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define MUL_PLATFORM_LINUX
	#error "Linux is not supported!"
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif // End of platform detection

#ifdef MUL_DEBUG
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

#ifdef MUL_DEBUG_ASSERT
	#define MUL_ASSERT(x, ...) { if(!(x)) { MUL_ERROR("Assertion Failed: {0}", __VA_ARGS__); MUL_DEBUGBREAK(); } }
	#define MUL_CORE_ASSERT(x, ...) { if(!(x)) { MUL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); MUL_DEBUGBREAK(); } }
#else
	#define MUL_ASSERT(x, ...)
	#define MUL_CORE_ASSERT(x, ...)
#endif

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