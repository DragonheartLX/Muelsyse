#pragma once
#include <Muelsyse.h>

namespace mul
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void setContext(const Ref<Scene>& scene);

		void onImGuiRender();

		Entity getSelectedEntity() const { return m_SelectionContext; }
		void setSelectedEntity(Entity entity);
	private:
		void drawEntityNode(Entity entity, std::string& tag);
		void drawComponents(Entity entity);

		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}