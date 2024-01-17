#pragma once
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Window/Window.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/Layer/LayerStack.h"
#include "Muelsyse/Event/Event.h"
#include "Muelsyse/ImGui/ImGuiLayer.h"

#ifdef MUL_NO_CONSOLE
	int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
#else
	int main(int argc, char** argv);
#endif

namespace mul
{
	class MUL_API Application
	{
	public:
		Application(const std::string& name = "Flowing Application");
		virtual ~Application();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		Window& getWindow() { return *m_Window; }

		void close();

		ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }

		static Application& get() { return *s_Instance; }
	private:
		void m_Run();

		bool m_OnWindowClose(WindowCloseEvent& e);
		bool m_OnWindowResize(WindowResizeEvent& e);
		
		ImGuiLayer* m_ImGuiLayer;
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;

		#ifdef MUL_NO_CONSOLE
			friend int APIENTRY ::WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
		#else
			friend int ::main(int argc, char** argv);
		#endif
	};

	Application* createApplication();
}