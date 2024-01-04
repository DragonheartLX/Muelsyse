#pragma once
#include "mulpch.h"
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Event/Event.h"

namespace mul 
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps
		(
			const std::string& title = "Muelsyse",
			unsigned int width = 1920,
			unsigned int height = 1080
		):
			Title(title), Width(width), Height(height)
		{

		}
	};

	class MUL_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}