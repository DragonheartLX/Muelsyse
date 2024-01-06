#include "mulpch.h"
#include "Muelsyse/Renderer/RenderCommand.h"

#include "Muelsyse/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace mul
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::create();
}