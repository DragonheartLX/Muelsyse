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
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			MUL_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	struct ApplicationSpecification
	{
		std::string Name = "Flowing Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class MUL_API Application
	{
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();

		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* layer);

		Window& getWindow() { return *m_Window; }

		void close();

		ImGuiLayer* getImGuiLayer() { return m_ImGuiLayer; }

		static Application& get() { return *s_Instance; }

		const ApplicationSpecification& getSpecification() const { return m_Specification; }

		void submitToMainThread(const std::function<void()>& function);
	private:
		void m_Run();

		bool m_OnWindowClose(WindowCloseEvent& e);
		bool m_OnWindowResize(WindowResizeEvent& e);
		
		void executeMainThreadQueue();

		ApplicationSpecification m_Specification;
		ImGuiLayer* m_ImGuiLayer;
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;
		
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;

		static Application* s_Instance;

		#ifdef MUL_NO_CONSOLE
			friend int APIENTRY ::WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow);
		#else
			friend int ::main(int argc, char** argv);
		#endif
	};

	Application* createApplication(ApplicationCommandLineArgs args);
}