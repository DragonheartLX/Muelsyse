#pragma once
#include "Muelsyse/Asset/AssetMetadata.h"

namespace mul 
{
	class AssetImporter
	{
	public:
		static Ref<Asset> importAsset(AssetHandle handle, const AssetMetadata& metadata);
	};
}