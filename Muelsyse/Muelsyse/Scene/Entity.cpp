#include "mulpch.h"
#include "Muelsyse/Scene/Entity.h"

namespace mul
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}