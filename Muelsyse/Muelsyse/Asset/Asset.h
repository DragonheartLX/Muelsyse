#pragma once
#include "Muelsyse/Core/UUID.h"

namespace mul 
{
	using AssetHandle = UUID;

	enum class AssetType: uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};
	
	std::string_view AssetTypeToString(AssetType type);
	AssetType AssetTypeFromString(std::string_view assetType);

	class Asset
	{
	public:
		AssetHandle Handle; // Generate handle

		virtual AssetType getType() const = 0;
	};
}