#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Window/Window.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/Layer/LayerStack.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/ImGui/ImGuiLayer.h"

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

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	};

	Application* createApplication();
}