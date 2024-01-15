#pragma once
#include "Muelsyse/Scene/Entity.h"

namespace mul
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		T& getComponent()
		{
			return m_Entity.getComponent<T>();
		}
	protected:
		virtual void onCreate() {}
		virtual void onDestroy() {}
		virtual void onUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}

