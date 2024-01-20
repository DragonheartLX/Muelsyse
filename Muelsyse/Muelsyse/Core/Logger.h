#pragma once
#include "mulpch.h"
#include "Muelsyse/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace mul
{
	class MUL_API Logger
	{
	public:
		static void init();

		static Ref<spdlog::logger>& getCoreLogger() { return m_CoreLogger; };
		static Ref<spdlog::logger>& getClientLogger() { return m_ClientLogger; };
	private:
		static Ref<spdlog::logger> m_CoreLogger;
		static Ref<spdlog::logger> m_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#ifdef MUL_LOGGER
	#define MUL_CORE_TRACE(...)		::mul::Logger::getCoreLogger()->trace(__VA_ARGS__)
	#define MUL_CORE_INFO(...)		::mul::Logger::getCoreLogger()->info(__VA_ARGS__)
	#define MUL_CORE_WARN(...)		::mul::Logger::getCoreLogger()->warn(__VA_ARGS__)
	#define MUL_CORE_ERROR(...)		::mul::Logger::getCoreLogger()->error(__VA_ARGS__)
	#define MUL_CORE_CRITICAL(...)	::mul::Logger::getCoreLogger()->critical(__VA_ARGS__)

	#define MUL_TRACE(...)			::mul::Logger::getClientLogger()->trace(__VA_ARGS__)
	#define MUL_INFO(...)			::mul::Logger::getClientLogger()->info(__VA_ARGS__)
	#define MUL_WARN(...)			::mul::Logger::getClientLogger()->warn(__VA_ARGS__)
	#define MUL_ERROR(...)			::mul::Logger::getClientLogger()->error(__VA_ARGS__)
	#define MUL_CRITICAL(...)		::mul::Logger::getClientLogger()->critical(__VA_ARGS__)
#else
	#define MUL_CORE_TRACE(...)
	#define MUL_CORE_INFO(...)
	#define MUL_CORE_WARN(...)
	#define MUL_CORE_ERROR(...)
	#define MUL_CORE_CRITICAL(...)

	#define MUL_TRACE(...)	
	#define MUL_INFO(...)	
	#define MUL_WARN(...)	
	#define MUL_ERROR(...)	
	#define MUL_CRITICAL(...)	
#endif
