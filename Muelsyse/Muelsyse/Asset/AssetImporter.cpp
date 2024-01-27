#include "mulpch.h"
#include "Muelsyse/Asset/AssetImporter.h"
#include "Muelsyse/Asset/TextureImporter.h"

#include <map>

namespace mul
{
	using AssetImportFunction = std::function<Ref<Asset>(AssetHandle, const AssetMetadata&)>;
	static std::map<AssetType, AssetImportFunction> s_AssetImportFunctions = 
    {
		{ AssetType::Texture2D, TextureImporter::importTexture2D }
	};

	Ref<Asset> AssetImporter::importAsset(AssetHandle handle, const AssetMetadata& metadata)
	{
		if (s_AssetImportFunctions.find(metadata.Type) == s_AssetImportFunctions.end())
		{
			MUL_CORE_ERROR("No importer available for asset type: {}", (uint16_t)metadata.Type);
			return nullptr;
		}

		return s_AssetImportFunctions.at(metadata.Type)(handle, metadata);
	}
}