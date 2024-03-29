#include "mulpch.h"
#include "Muelsyse/Asset/EditorAssetManager.h"
#include "Muelsyse/Asset/AssetImporter.h"
#include "Muelsyse/Project/Project.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace mul
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const std::string_view& v)
	{
		out << std::string(v.data(), v.size());
		return out;
	}

	bool EditorAssetManager::isAssetHandleValid(AssetHandle handle) const
	{
		return handle != 0 && m_AssetRegistry.find(handle) != m_AssetRegistry.end();
	}

	bool EditorAssetManager::isAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAssets.find(handle) != m_LoadedAssets.end();
	}

	void EditorAssetManager::importAsset(const std::filesystem::path& filepath)
	{
		AssetHandle handle; // generate new handle
		AssetMetadata metadata;
		metadata.FilePath = filepath;
		metadata.Type = AssetType::Texture2D; // TODO(Yan): grab this from extension and try to load
		Ref<Asset> asset = AssetImporter::importAsset(handle, metadata);
		asset->Handle = handle;
		if (asset)
		{
			m_LoadedAssets[handle] = asset;
			m_AssetRegistry[handle] = metadata;
			serializeAssetRegistry();
		}
	}

	const AssetMetadata& EditorAssetManager::getMetadata(AssetHandle handle) const
	{
		static AssetMetadata s_NullMetadata;
		auto it = m_AssetRegistry.find(handle);
		if (it == m_AssetRegistry.end())
			return s_NullMetadata;

		return it->second;
	}

	Ref<Asset> EditorAssetManager::getAsset(AssetHandle handle) const
	{
		// 1. check if handle is valid
		if (!isAssetHandleValid(handle))
			return nullptr;

		// 2. check if asset needs load (and if so, load)
		Ref<Asset> asset;
		if (isAssetLoaded(handle))
		{
			asset = m_LoadedAssets.at(handle);
		}
		else
		{
			// load asset
			const AssetMetadata& metadata = getMetadata(handle);
			asset = AssetImporter::importAsset(handle, metadata);
			if (!asset)
			{
				// import failed
				MUL_CORE_ERROR("EditorAssetManager::GetAsset - asset import failed!");
			} 
		}
		// 3. return asset
		return asset;
	}

	void EditorAssetManager::serializeAssetRegistry()
	{
		auto path = Project::getAssetRegistryPath();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "AssetRegistry" << YAML::Value;

			out << YAML::BeginSeq;
			for (const auto&[handle, metadata] : m_AssetRegistry)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << handle;
				std::string filepathStr = metadata.FilePath.generic_string();
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(metadata.Type);
				out << YAML::EndMap;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(path);
		fout << out.c_str();

	}

	bool EditorAssetManager::deserializeAssetRegistry()
	{
		auto path = Project::getAssetRegistryPath();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			MUL_CORE_ERROR("Failed to load project file '{0}'\n     {1}", path.string(), e.what());
			return false;
		}

		auto rootNode = data["AssetRegistry"];
		if (!rootNode)
			return false;

		for (const auto& node : rootNode)
		{
			AssetHandle handle = node["Handle"].as<uint64_t>();
			auto& metadata = m_AssetRegistry[handle];
			metadata.FilePath = node["FilePath"].as<std::string>();
			metadata.Type = AssetTypeFromString(node["Type"].as<std::string>());
		}

		return true;
	}
}
