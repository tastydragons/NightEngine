#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Night
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}