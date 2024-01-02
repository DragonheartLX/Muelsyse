#pragma once
#include "Core/Core.h"

namespace mul
{
	class MUL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:

	};

	Application* createApplication();
}