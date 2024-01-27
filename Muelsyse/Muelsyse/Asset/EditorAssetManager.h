#pragma once
#include "Muelsyse/Asset/AssetManagerBase.h"
#include "Muelsyse/Asset/AssetMetadata.h"

#include <map>

namespace mul 
{
	using AssetRegistry = std::map<AssetHandle, AssetMetadata>;

	class EditorAssetManager: public AssetManagerBase
	{
	public:
		~EditorAssetManager() = default;
		virtual Ref<Asset> getAsset(AssetHandle handle) const override;

		virtual bool isAssetHandleValid(AssetHandle handle) const override;
		virtual bool isAssetLoaded(AssetHandle handle) const override;

		void importAsset(const std::filesystem::path& filepath);

		const AssetMetadata& getMetadata(AssetHandle handle) const;

		const AssetRegistry& getAssetRegistry() const { return m_AssetRegistry; }

		void serializeAssetRegistry();
		bool deserializeAssetRegistry();
	private:
		AssetRegistry m_AssetRegistry;
		AssetMap m_LoadedAssets;

		// TODO: memory-only assets
	};
}