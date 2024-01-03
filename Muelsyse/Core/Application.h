#pragma once
#include "Core/Core.h"
#include "Window/Window.h"
#include "Event/ApplicationEvent.h"
#include "Layer/LayerStack.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

namespace mul
{
	class MUL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		inline Window& getWindow() { return *m_Window; }

		inline static Application& get() { return *s_Instance; }
	private:
		bool m_OnWindowClose(WindowCloseEvent& e);

		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
		static Application* s_Instance;
	};

	Application* createApplication();
}