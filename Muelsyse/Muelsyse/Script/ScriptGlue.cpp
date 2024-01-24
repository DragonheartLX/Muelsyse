#include "mulpch.h"
#include "Muelsyse/Script/ScriptGlue.h"
#include "Muelsyse/Script/ScriptEngine.h"

#include "Muelsyse/Core/UUID.h"
#include "Muelsyse/Input/KeyCode.h"
#include "Muelsyse/Input/Input.h"

#include "Muelsyse/Scene/Scene.h"
#include "Muelsyse/Scene/Entity.h"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include "box2d/b2_body.h"

namespace mul
{
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	#define MUL_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Muelsyse.InternalCalls::"#Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		MUL_CORE_WARN("Value: {0}, {1}, {2}", parameter->x, parameter->y, parameter->z);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		MUL_CORE_WARN("Value: {0}, {1}, {2}", parameter->x, parameter->y, parameter->z);
		return glm::dot(*parameter, *parameter);
	}

	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::getManagedInstance(entityID);
	}

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->getEntityByUUID(entityID);
		MUL_CORE_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		MUL_CORE_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->findEntityByName(nameCStr);
		mono_free(nameCStr);

		if (!entity)
			return 0;
		
		return entity.getUUID();
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->getEntityByUUID(entityID);
		MUL_CORE_ASSERT(entity);

		*outTranslation = entity.getComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->getEntityByUUID(entityID);
		MUL_CORE_ASSERT(entity);

		entity.getComponent<TransformComponent>().Translation = *translation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->getEntityByUUID(entityID);
		MUL_CORE_ASSERT(entity);

		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::getSceneContext();
		MUL_CORE_ASSERT(scene);
		Entity entity = scene->getEntityByUUID(entityID);
		MUL_CORE_ASSERT(entity);

		auto& rb2d = entity.getComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}
	
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::isKeyPressed(keycode);
	}

	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
		{
			std::string_view typeName = typeid(Component).name();
			size_t pos = typeName.find_last_of(':');
			std::string_view structName = typeName.substr(pos + 1);
			std::string managedTypename = fmt::format("Muelsyse.{}", structName);

			MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::getCoreAssemblyImage());
			if (!managedType)
			{
				MUL_CORE_ERROR("Could not find component type {}", managedTypename);
				return;
			}
			s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.hasComponent<Component>(); };
		}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::registerComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::registerFunctions()
	{
		MUL_ADD_INTERNAL_CALL(NativeLog);
		MUL_ADD_INTERNAL_CALL(NativeLog_Vector);
		MUL_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		MUL_ADD_INTERNAL_CALL(GetScriptInstance);

		MUL_ADD_INTERNAL_CALL(Entity_HasComponent);
		MUL_ADD_INTERNAL_CALL(Entity_FindEntityByName);
		
		MUL_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		MUL_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		
		MUL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		MUL_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		MUL_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}