#pragma once
#include "Muelsyse/Asset/Asset.h"

#include <filesystem>

namespace mul
{
	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path FilePath;

		operator bool() const { return Type != AssetType::None; }
	};
}