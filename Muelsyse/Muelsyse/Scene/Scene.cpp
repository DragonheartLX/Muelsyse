#include "mulpch.h"
#include "Muelsyse/Scene/Scene.h"
#include "Muelsyse/Scene/Components.h"
#include "Muelsyse/Renderer/Renderer2D.h"
#include "Muelsyse/Scene/Entity.h"

#include <glm/glm.hpp>

namespace mul
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::createEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::destroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::onUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=, this](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity{ entity, this };
					nsc.Instance->onCreate();
				}

				nsc.Instance->onUpdate(ts);
			});
		}
		
		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;

		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.getTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(*mainCamera, cameraTransform);

			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
			}

			Renderer2D::endScene();
		}
	}

	void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);

		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
		}

		Renderer2D::endScene();
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.setViewportSize(width, height);
		}

	}

	Entity Scene::getPrimaryCameraEntity()
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{entity, this};
		}
		return {};
	}

	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void MUL_API Scene::onComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.setViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void MUL_API Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}