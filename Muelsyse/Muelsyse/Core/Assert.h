#pragma once
#include "Muelsyse/Core/Core.h"

#include <filesystem>

#ifdef MUL_DEBUG_ASSERT
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define MUL_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { MUL##type##ERROR(msg, __VA_ARGS__); MUL_DEBUGBREAK(); } }
	#define MUL_INTERNAL_ASSERT_WITH_MSG(type, check, ...) MUL_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define MUL_INTERNAL_ASSERT_NO_MSG(type, check) MUL_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", MUL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define MUL_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define MUL_INTERNAL_ASSERT_GET_MACRO(...) MUL_EXPAND_MACRO( MUL_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, MUL_INTERNAL_ASSERT_WITH_MSG, MUL_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define MUL_ASSERT(...) MUL_EXPAND_MACRO( MUL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define MUL_CORE_ASSERT(...) MUL_EXPAND_MACRO( MUL_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define MUL_ASSERT(...)
	#define MUL_CORE_ASSERT(...)
#endif

#ifdef MUL_ENABLE_VERIFY

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define MUL_INTERNAL_VERIFY_IMPL(type, check, msg, ...) { if(!(check)) { MUL##type##ERROR(msg, __VA_ARGS__); MUL_DEBUGBREAK(); } }
#define MUL_INTERNAL_VERIFY_WITH_MSG(type, check, ...) MUL_INTERNAL_VERIFY_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define MUL_INTERNAL_VERIFY_NO_MSG(type, check) MUL_INTERNAL_VERIFY_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", MUL_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define MUL_INTERNAL_VERIFY_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define MUL_INTERNAL_VERIFY_GET_MACRO(...) MUL_EXPAND_MACRO( MUL_INTERNAL_VERIFY_GET_MACRO_NAME(__VA_ARGS__, MUL_INTERNAL_VERIFY_WITH_MSG, MUL_INTERNAL_VERIFY_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define MUL_VERIFY(...) MUL_EXPAND_MACRO( MUL_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define MUL_CORE_VERIFY(...) MUL_EXPAND_MACRO( MUL_INTERNAL_VERIFY_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define MUL_VERIFY(...)
#define MUL_CORE_VERIFY(...)
#endif