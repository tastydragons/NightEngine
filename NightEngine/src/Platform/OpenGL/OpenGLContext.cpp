#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Night
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		NIGHT_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		NIGHT_CORE_ASSERT(status, "Failed to initialize Glad!");

		NIGHT_CORE_INFO("OpenGl Info:");
		NIGHT_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		NIGHT_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		NIGHT_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));
	}
	
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}