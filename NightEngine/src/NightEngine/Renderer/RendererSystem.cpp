#include "RendererSystem.h"

namespace Night {
	void RendererSystem::Init()
	{
		Character::LoadTexture();
		m_Background = CreateRef<Texture>("Assets/Textures/Checkerboard.png");

		Renderer::Init();
		Font::Init();
	}

	void RendererSystem::Update()
	{
		Renderer::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Renderer::Clear();

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_CameraController.SetZoomLevel(1.f);

		Renderer::EndScene();
	}

	void RendererSystem::End()
	{
		Renderer::Shutdown();
	}
}