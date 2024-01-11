#include "Muelsyse/Core/Application.h"
#include "Muelsyse/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace mul
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		MUL_PROFILE_FUNCTION();

		MUL_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Window::create(WindowProps(name));
		m_Window->setEventCallback(MUL_BIND_EVENT_FUNC(Application::onEvent));

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		MUL_PROFILE_FUNCTION();
		Renderer::shutdown();
	}

	void Application::m_Run()
	{
		MUL_PROFILE_FUNCTION();

		while (m_Running)
		{
			MUL_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

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
}