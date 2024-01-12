#pragma once
#include "Muelsyse/Core/Timestep.h"

#include <entt/entt.hpp>

namespace mul
{
	class MUL_API Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity createEntity();

		// TEMP
		entt::registry& reg() { return m_Registry; }

		void onUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
	};
}