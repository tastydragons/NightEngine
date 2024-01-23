#pragma once

#include "Night.h"

class Sandbox2D : public Night::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Night::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Night::Event& e) override;
private:
	Night::OrthographicCameraController m_CameraController;

	// Temp
	Night::Ref<Night::VertexArray> m_SquareVA;
	Night::Ref<Night::Shader> m_FlatColorShader;

	Night::Ref<Night::Texture2D> m_CheckerboardTexture;

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};