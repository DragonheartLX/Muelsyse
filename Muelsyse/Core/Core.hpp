#pragma once

#ifdef MUL_PLATFORM_WINDOWS
	#ifdef MUL_BUILD_SHARED
		#ifdef _MSC_VER
			#ifdef MUL_BUILD_DLL
				#define MUL_API __declspec(dllexport)
			#else
				#define MUL_API __declspec(dllimport)
			#endif
		#else
			#define MUL_API
		#endif
	#endif
#else
	#error Muelsyse currently only support windows with MSVC
#endif