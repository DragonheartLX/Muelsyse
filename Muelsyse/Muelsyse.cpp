#include "Muelsyse.hpp"
#include <spdlog/spdlog.h>

namespace mls
{
	void MuelsyseVersion()
	{
		spdlog::set_pattern("[%Y-%m-%d %T]%^[%l]%$ %v");
		spdlog::info("0.0.0dev");
	};
} // namespace mls
