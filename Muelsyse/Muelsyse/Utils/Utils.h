#pragma once
#include "Muelsyse/Core/Core.h"

#include <string>
#include <optional>

namespace mul
{
	class MUL_API FileDialogs
	{
	public:
		// These return empty strings if cancelled
		// TODO: arguments current not use
		static std::optional<std::string> openFile(const char* filterDes, const char* filter);
		static std::optional<std::string> saveFile(const char* filterDes, const char* filter);
	};
}