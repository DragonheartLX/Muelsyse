#pragma once
#include "Muelsyse/Core/Timestep.h"
#include "Muelsyse/Scene/EditorCamera.h"
#include "Muelsyse/Core/UUID.h"

#include <entt/entt.hpp>

class b2World;

namespace mul
{
	class Entity;

	class MUL_API Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> copy(Ref<Scene> other);

		Entity createEntity(const std::string& name = std::string());
		Entity createEntityWithUUID(UUID uuid, const std::string& name = std::string());

		void destroyEntity(Entity entity);

		void onRuntimeStart();
		void onRuntimeStop();

		void onSimulationStart();
		void onSimulationStop();

		void onUpdateRuntime(Timestep ts);
		void onUpdateSimulation(Timestep ts, EditorCamera& camera);
		void onUpdateEditor(Timestep ts, EditorCamera& camera);
		void onViewportResize(uint32_t width, uint32_t height);

		Entity duplicateEntity(Entity entity);

		Entity findEntityByName(std::string_view name);
		Entity getEntityByUUID(UUID uuid);

		Entity getPrimaryCameraEntity();

		bool isRunning() const { return m_IsRunning; }

		template<typename... Components>
		auto getAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		template<typename T>
		void onComponentAdded(Entity entity, T& component);

		void onPhysics2DStart();
		void onPhysics2DStop();

		void renderScene(EditorCamera& camera);

		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsRunning = false;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
		
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}