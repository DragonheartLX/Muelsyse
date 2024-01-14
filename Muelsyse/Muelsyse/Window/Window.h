#pragma once
#include "mulpch.h"
#include "Muelsyse/Core/Core.h"
#include "Muelsyse/Event/Event.h"

namespace mul 
{
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& winTitle = "Muelsyse", uint32_t winWidth = 1920,	uint32_t winHeight = 1080):
			title(winTitle), width(winWidth), height(winHeight)
		{

		}
	};

	class MUL_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFunc& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Scope<Window> create(const WindowProps& props = WindowProps());
	};
}