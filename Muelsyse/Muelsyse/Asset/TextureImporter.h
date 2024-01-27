#pragma once
#include "Muelsyse/Asset/Asset.h"
#include "Muelsyse/Asset/AssetMetadata.h"
#include "Muelsyse/Renderer/Texture.h"

namespace mul
{
	class TextureImporter
	{
	public:
		// AssetMetadata filepath is relative to project asset directory
		static Ref<Texture2D> importTexture2D(AssetHandle handle, const AssetMetadata& metadata);

		// Reads file directly from filesystem
		// (i.e. path has to be relative / absolute to working directory)
		static Ref<Texture2D> loadTexture2D(const std::filesystem::path& path);
	};
}