#pragma once
#include "Muelsyse/Core/Buffer.h"

namespace mul
{
	class FileSystem
	{
	public:
		// TODO: move to FileSystem class
		static Buffer readFileBinary(const std::filesystem::path& filepath);
	};
}