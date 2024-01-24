#include "mulpch.h"
#include "Muelsyse/Renderer/Renderer2D.h"
#include "Muelsyse/Scene/Scene.h"
#include "Muelsyse/Scene/Components.h"
#include "Muelsyse/Scene/Entity.h"
#include "Muelsyse/Scene/ScriptableEntity.h"
#include "Muelsyse/Physics/Physics2D.h"
#include "Muelsyse/Script/ScriptEngine.h"

#include <glm/glm.hpp>
// Box2D
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

namespace mul
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	template<typename... Component>
	static void copyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}

	template<typename... Component>
	static void copyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		copyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void copyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.hasComponent<Component>())
				dst.addOrReplaceComponent<Component>(src.getComponent<Component>());
		}(), ...);
	}

	template<typename... Component>
	static void copyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		copyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = createRef<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).Tag;
			Entity newEntity = newScene->createEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		copyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
	}

	Entity Scene::createEntity(const std::string& name)
	{
		return createEntityWithUUID(UUID(), name);
	}

	Entity Scene::createEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.addComponent<IDComponent>(uuid);
		entity.addComponent<TransformComponent>();
		auto& tag = entity.addComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;
		
		return entity;
	}

	void Scene::destroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
		m_EntityMap.erase(entity.getUUID());
	}

	void Scene::onRuntimeStart()
	{
		m_IsRunning = true;

		onPhysics2DStart();

		// Scripting
		{
			ScriptEngine::onRuntimeStart(this);
			// Instantiate all script entities

			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::onCreateEntity(entity);
			}
		}
	}

	void Scene::onRuntimeStop()
	{
		m_IsRunning = false;
		
		onPhysics2DStop();

		ScriptEngine::onRuntimeStop();
	}

	void Scene::onSimulationStart()
	{
		onPhysics2DStart();
	}

	void Scene::onSimulationStop()
	{
		onPhysics2DStop();
	}

	void Scene::onUpdateRuntime(Timestep ts)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Update scripts
			{
				// C# Entity OnUpdate
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptEngine::onUpdateEntity(entity, ts);
				}

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

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.getComponent<TransformComponent>();
					auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;

					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
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
					cameraTransform = transform.getTransform(camera.FixedRotation);
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::beginScene(*mainCamera, cameraTransform);

			// Draw sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::drawCircle(transform.getTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}

			Renderer2D::endScene();
		}
	}

	void Scene::onUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<Rigidbody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.getComponent<TransformComponent>();
					auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}

		// Render
		renderScene(camera);
	}

	void Scene::onUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		// Render
		renderScene(camera);
	}

	void Scene::onViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;
		
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

	void Scene::step(int frames)
	{
		m_StepFrames = frames;
	}

	Entity Scene::duplicateEntity(Entity entity)
	{
		// Copy name because we're going to modify component data structure
		std::string name = entity.getName();
		Entity newEntity = createEntity(name);
		copyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::findEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::getEntityByUUID(UUID uuid)
	{
		// TODO(Yan): Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}

	void Scene::onPhysics2DStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		auto view = m_Registry.view<Rigidbody2DComponent>();
		for (auto e : view)
		{
			Entity entity = { e, this };
			auto& transform = entity.getComponent<TransformComponent>();
			auto& rb2d = entity.getComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.hasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.getComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.hasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.getComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::onPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::renderScene(EditorCamera& camera)
	{
		Renderer2D::beginScene(camera);

		// Draw sprites
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::drawSprite(transform.getTransform(), sprite, (int)entity);
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::drawCircle(transform.getTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
		}

		Renderer2D::endScene();
	}

	template<typename T>
	void Scene::onComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void MUL_API Scene::onComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
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
	void MUL_API Scene::onComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
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

	template<>
	void MUL_API Scene::onComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void MUL_API Scene::onComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}
}