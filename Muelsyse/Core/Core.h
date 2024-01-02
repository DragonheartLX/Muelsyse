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

#define BIT(x) (1 << x)