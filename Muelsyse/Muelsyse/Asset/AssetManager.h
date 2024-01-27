#pragma once
#include "Muelsyse/Asset/AssetManagerBase.h"
#include "Muelsyse/Project/Project.h"

namespace mul
{
	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> getAsset(AssetHandle handle)
		{
			Ref<Asset> asset = Project::getActive()->getAssetManager()->getAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}
	};
}