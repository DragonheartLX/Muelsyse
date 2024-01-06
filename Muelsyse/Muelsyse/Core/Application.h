#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Window/Window.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/Layer/LayerStack.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace mul
{
	class MUL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }

		inline static Application& get() { return *s_Instance; }
	private:
		void m_Run();

		bool m_OnWindowClose(WindowCloseEvent& e);
		bool m_OnWindowResize(WindowResizeEvent& e);
		
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;

		friend int ::main(int argc, char** argv);
	};

	Application* createApplication();
}