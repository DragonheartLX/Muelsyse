#pragma once
#include "Muelsyse/Project/Project.h"

namespace mul
{
	class ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Project> project);

		bool serialize(const std::filesystem::path& filepath);
		bool deserialize(const std::filesystem::path& filepath);
	private:
		Ref<Project> m_Project;
	};
}
