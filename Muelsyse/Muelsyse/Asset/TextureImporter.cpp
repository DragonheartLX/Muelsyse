#include "mulpch.h"
#include "Muelsyse/Asset/TextureImporter.h"
#include "Muelsyse/Core/Buffer.h"
#include "Muelsyse/Project/Project.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace mul
{
	Ref<Texture2D> TextureImporter::importTexture2D(AssetHandle handle, const AssetMetadata& metadata)
	{
		MUL_PROFILE_FUNCTION();

		return loadTexture2D(Project::getAssetDirectory() / metadata.FilePath);
	}

	Ref<Texture2D> TextureImporter::loadTexture2D(const std::filesystem::path& path)
	{
		MUL_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		Buffer data;

		{
			MUL_PROFILE_SCOPE("stbi_load - TextureImporter::ImportTexture2D");
			std::string pathStr = path.string();
			data.Data = stbi_load(pathStr.c_str(), &width, &height, &channels, 0);
		}

		if (data.Data == nullptr)
		{
			MUL_CORE_ERROR("TextureImporter::ImportTexture2D - Could not load texture from filepath: {}", path.string());
			return nullptr;
		}

		// TODO: think about this
		data.Size = width * height * channels;

		TextureSpecification spec;
		spec.Width = width;
		spec.Height = height;
		switch (channels)
		{
			case 3:
				spec.Format = ImageFormat::RGB8;
				break;
			case 4:
				spec.Format = ImageFormat::RGBA8;
				break;
		}

		Ref<Texture2D> texture = Texture2D::create(spec, data);
		data.release();
		return texture;
	}

}
