#pragma once
#include "Muelsyse/Asset/Asset.h"

#include <map>

namespace mul
{
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;

	class AssetManagerBase
	{
	public:
		virtual Ref<Asset> getAsset(AssetHandle handle) const = 0;

		virtual bool isAssetHandleValid(AssetHandle handle) const = 0;
		virtual bool isAssetLoaded(AssetHandle handle) const = 0;
	};
}