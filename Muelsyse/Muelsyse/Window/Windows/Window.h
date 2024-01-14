#pragma once
#include "Muelsyse/Window/Window.h"
#include "Muelsyse/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace mul
{
	class WindowsWindow: public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;

		unsigned int getWidth() const override { return m_Data.width; }
		unsigned int getHeight() const override { return m_Data.height; }

		void setEventCallback(const EventCallbackFunc& callback) override { m_Data.eventCallback = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		virtual void* getNativeWindow() const override { return m_Window; }
	private:
		virtual void init(const WindowProps& props);
		virtual void shutdown();

		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFunc eventCallback;
		};

		WindowData m_Data;
	};
}