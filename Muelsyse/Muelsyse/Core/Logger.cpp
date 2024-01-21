#include "Muelsyse/Core/Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace mul
{
	Ref<spdlog::logger> Logger::m_CoreLogger;
	Ref<spdlog::logger> Logger::m_ClientLogger;

	void Logger::init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		// Logger init before Application
		// So workDir unchanged
		// log file will save in exe file folder
		// TODO: change to workDir?
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Muelsyse.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		m_CoreLogger = std::make_shared<spdlog::logger>("Muelsyse", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_CoreLogger);
		m_CoreLogger->set_level(spdlog::level::trace);
		m_CoreLogger->flush_on(spdlog::level::trace);

		m_ClientLogger = std::make_shared<spdlog::logger>("Flowing", begin(logSinks), end(logSinks));
		spdlog::register_logger(m_ClientLogger);
		m_ClientLogger->set_level(spdlog::level::trace);
		m_ClientLogger->flush_on(spdlog::level::trace);
	}
}