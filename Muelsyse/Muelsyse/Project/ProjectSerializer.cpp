#include "mulpch.h"
#include "Muelsyse/Project/ProjectSerializer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace mul
{
	ProjectSerializer::ProjectSerializer(Ref<Project> project): 
        m_Project(project)
	{
	}

	bool ProjectSerializer::serialize(const std::filesystem::path& filepath)
	{
		const auto& config = m_Project->getConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap;// Project
				out << YAML::Key << "Name" << YAML::Value << config.Name;
				out << YAML::Key << "EngineVersion" << YAML::Value << MUL_VERSION_STRING;
				out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
				out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(filepath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::deserialize(const std::filesystem::path& filepath)
	{
		auto& config = m_Project->getConfig();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath.string());
		}
		catch (YAML::ParserException e)
		{
			MUL_CORE_ERROR("Failed to load project file '{0}'\n     {1}", filepath.string().c_str(), e.what());
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
			return false;

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
		return true;
	}
}
