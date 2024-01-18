#pragma once
#include "Muelsyse/Core/Core.h"

#include <string>

namespace mul
{
	class MUL_API FileDialogs
	{
	public:
		// These return empty strings if cancelled
		// TODO: arguments current not use
		static std::string openFile(const char* filterDes, const char* filter);
		static std::string saveFile(const char* filterDes, const char* filter);
	};
}