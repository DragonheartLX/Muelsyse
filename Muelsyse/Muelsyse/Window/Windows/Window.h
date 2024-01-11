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

		unsigned int getWidth() const override { return m_Data.Width; }
		unsigned int getHeight() const override { return m_Data.Height; }

		void setEventCallback(const EventCallbackFunc& callback) override { m_Data.EventCallback = callback; }
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
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFunc EventCallback;
		};

		WindowData m_Data;
	};
}