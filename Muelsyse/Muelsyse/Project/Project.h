#pragma once
#include "Muelsyse/Core/Core.h"
// #include "Muelsyse/Asset/RuntimeAssetManager.h"
// #include "Muelsyse/Asset/EditorAssetManager.h"

#include <string>
#include <filesystem>

namespace mul
{
	struct ProjectConfig
	{
		std::string Name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModulePath;
	};

	class Project
	{
	public:
		static const std::filesystem::path& getProjectDirectory()
		{
			MUL_CORE_ASSERT(s_ActiveProject);
			return s_ActiveProject->m_ProjectDirectory;
		}

		static std::filesystem::path getAssetDirectory()
		{
			MUL_CORE_ASSERT(s_ActiveProject);
			return getProjectDirectory() / s_ActiveProject->m_Config.AssetDirectory;
		}

		// TODO: move to asset manager when have one
		static std::filesystem::path getAssetFileSystemPath(const std::filesystem::path& path)
		{
			MUL_CORE_ASSERT(s_ActiveProject);
			return getAssetDirectory() / path;
		}

		ProjectConfig& getConfig() { return m_Config; }

		static Ref<Project> getActive() { return s_ActiveProject; }
		// std::shared_ptr<AssetManagerBase> getAssetManager() { return m_AssetManager; }
		// std::shared_ptr<RuntimeAssetManager> getRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_AssetManager); }
		// std::shared_ptr<EditorAssetManager> getEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_AssetManager); }

		static Ref<Project> newProject();
		static Ref<Project> loadProject(const std::filesystem::path& path);
		static bool saveActive(const std::filesystem::path& path);
	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;
		// std::shared_ptr<AssetManagerBase> m_AssetManager;

		inline static Ref<Project> s_ActiveProject;
	};
}
