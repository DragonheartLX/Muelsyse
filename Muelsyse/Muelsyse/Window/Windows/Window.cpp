#include "mulpch.h"
#include "Muelsyse/Window/Windows/Window.h"
#include "Muelsyse/Event/ApplicationEvent.h"
#include "Muelsyse/Event/MouseEvent.h"
#include "Muelsyse/Event/KeyEvent.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLContext.h"
#include "Muelsyse/Renderer/Renderer.h"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace mul 
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		MUL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		MUL_PROFILE_FUNCTION();

		init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		MUL_PROFILE_FUNCTION();

		shutdown();
	}

	void WindowsWindow::init(const WindowProps& props)
	{
		MUL_PROFILE_FUNCTION();

		m_Data.title = props.title;
		m_Data.width = props.width;
		m_Data.height = props.height;

		MUL_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (s_GLFWWindowCount == 0)
		{
			MUL_PROFILE_SCOPE("glfwInit");

			MUL_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			MUL_CORE_ASSERT(success, "Could not initialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			MUL_PROFILE_SCOPE("glfwCreateWindow");

		#if defined(MUL_DEBUG)
			if (Renderer::getAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif

			m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}
		
		m_Context = GraphicsContext::create(m_Window);
		m_Context->init();
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		setVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				WindowResizeEvent event(width, height);
				data.eventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.eventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.eventCallback(event);
						break;
					}
				case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.eventCallback(event);
						break;
					}
				case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.eventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.eventCallback(event);
						break;
					}
				case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.eventCallback(event);
			});
	}

	void WindowsWindow::shutdown()
	{
		MUL_PROFILE_FUNCTION();

		glfwDestroyWindow(m_Window);
		
		if (--s_GLFWWindowCount == 0)
		{
			MUL_CORE_INFO("Terminating GLFW");
			glfwTerminate();
		}
	}

	void WindowsWindow::onUpdate()
	{
		MUL_PROFILE_FUNCTION();

		glfwPollEvents();
		m_Context->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		MUL_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.vSync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_Data.vSync;
	}

}