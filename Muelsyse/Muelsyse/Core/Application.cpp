#include "Muelsyse/Core/Application.h"
#include "Muelsyse/Renderer/Renderer.h"
#include "Muelsyse/Utils/Utils.h"
#include "Muelsyse/Script/ScriptEngine.h"

namespace mul
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification): 
		m_Specification(specification)
	{
		MUL_PROFILE_FUNCTION();

		MUL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(m_Specification.WorkingDirectory);

		m_Window = Window::create(WindowProps(m_Specification.Name));
		m_Window->setEventCallback(MUL_BIND_EVENT_FUNC(Application::onEvent));

		Renderer::init();
		ScriptEngine::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MUL_PROFILE_FUNCTION();
		ScriptEngine::shutdown();
		Renderer::shutdown();
	}

	void Application::m_Run()
	{
		MUL_PROFILE_FUNCTION();

		while (m_Running)
		{
			MUL_PROFILE_SCOPE("RunLoop");

			float time = Time::getTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			executeMainThreadQueue();

			if(!m_Minimized)
			{
				{
					MUL_PROFILE_SCOPE("LayerStack OnUpdate");
				
					for (Layer* layer : m_LayerStack)
						layer->onUpdate(timestep);
				}

				m_ImGuiLayer->begin();
				{
					MUL_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->onImGuiRender();
				}
				m_ImGuiLayer->end();
			}

			m_Window->onUpdate();
		}
	}

	void Application::onEvent(Event& e)
	{
		MUL_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.dispatcher<WindowCloseEvent>(MUL_BIND_EVENT_FUNC(Application::m_OnWindowClose));
		dispatcher.dispatcher<WindowResizeEvent>(MUL_BIND_EVENT_FUNC(Application::m_OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->onEvent(e);
		}
	}

	void Application::pushLayer(Layer* layer)
	{
		MUL_PROFILE_FUNCTION();
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* layer)
	{
		MUL_PROFILE_FUNCTION();
		m_LayerStack.pushOverlay(layer);
		layer->onAttach();
	}

	void Application::close()
	{
		m_Running = false;
	}

	void Application::submitToMainThread(const std::function<void()>& function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		m_MainThreadQueue.emplace_back(function);
	}

	bool Application::m_OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::m_OnWindowResize(WindowResizeEvent& e)
	{
		MUL_PROFILE_FUNCTION();

		if (e.getWidth() == 0 || e.getHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::onWindowResize(e.getWidth(), e.getHeight());

		return false;
	}

	void Application::executeMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto& func : m_MainThreadQueue)
			func();

		m_MainThreadQueue.clear();
	}
}