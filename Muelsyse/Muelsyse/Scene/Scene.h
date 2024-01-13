#pragma once
#include "Muelsyse/Core/Timestep.h"

#include <entt/entt.hpp>

namespace mul
{
	class Entity;

	class MUL_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity createEntity(const std::string& name = std::string());

		void onUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}