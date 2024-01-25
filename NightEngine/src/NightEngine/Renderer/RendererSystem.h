#pragma once

#include "hzpch.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/CameraController.h"
//#include "../Game/Character.h"

namespace Night {
	class RendererSystem {
	public:
		static void Init();
		static void Update();
		static void End();
		
	private:
		inline static Ref<Texture> m_Background;
		inline static CameraController m_CameraController{ 1920.0f / 1080.f };
	};
}