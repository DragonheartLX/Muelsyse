#pragma once
#include "Muelsyse/Core/Core.h"

namespace mul 
{
	class GraphicsContext
	{
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;

		static Scope<GraphicsContext> create(void* window);
	};

}