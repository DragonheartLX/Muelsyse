#include "mulpch.h"
#include "Muelsyse/Renderer/OpenGL/OpenGLContext.h"

#include "External/glad/glad.h"
#include <GLFW/glfw3.h>

namespace mul
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):
		m_WindowHandle(windowHandle)
	{
		MUL_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::init()
	{
		MUL_PROFILE_FUNCTION();
		
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MUL_CORE_ASSERT(status, "Failed to initialize Glad!");

		// fmt error when formatting const unsigned char*
		// https://github.com/gabime/spdlog/issues/2719
		// https://github.com/fmtlib/fmt/issues/3307
		MUL_CORE_INFO("=====OpenGL Info=====");
		MUL_CORE_INFO("Vendor: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		MUL_CORE_INFO("Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		MUL_CORE_INFO("Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
		MUL_CORE_INFO("=====================");

		// #ifdef MUL_ENABLE_ASSERTS
		// int versionMajor;
		// int versionMinor;
		// glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		// glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
// 
		// MUL_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Muelsyse requires at least OpenGL version 4.5!");
		// #endif
	}

	void OpenGLContext::swapBuffers()
	{
		MUL_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}