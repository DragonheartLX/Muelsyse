#include "mulpch.h"
#include "Muelsyse/Renderer/RenderCommand.h"

#include "Muelsyse/Renderer/OpenGL/OpenGLRendererAPI.h"

namespace mul
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}