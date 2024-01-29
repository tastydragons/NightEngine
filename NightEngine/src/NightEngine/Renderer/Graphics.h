#pragma once

#include "Platform/System/System.h"
#include "Platform/Windows/Window.h"

#include "NightEngine/Renderer/Texture.h"
#include "NightEngine/Renderer/Camera2D.h"
#include "NightEngine/Renderer/Renderer.h"
#include "NightEngine/Renderer/Buffer.h"
#include "NightEngine/Renderer/TextRendering.h"

#include "NightEngine/ECS/Components/Components.h"
#include "NightEngine/Animation/Animation.h"

#include "NightEngine/Math/NEMath.h"
#include "NightEngine/Math/Physics.h"

namespace Night
{
	class Graphic : public System
	{
	public:

		//For Debugging Purposes
		virtual std::string GetName() { return "graphic"; }

		virtual void Init() override;
		virtual void Update(float Frametime) override;
		virtual void End() override;

		inline static Camera2D camera{ -1.0f, 1.0f, -1.0f , 1.0f };
		inline static Camera2D scene_camera{ -1.0f, 1.0f, -1.0f , 1.0f };
		inline static Camera2D* mcamera = nullptr;
		bool misFps = false;
	private:
		//for testing
		std::unique_ptr<Renderer> mRenderer = std::make_unique<Renderer>();
		std::shared_ptr<Font> mFont = std::make_shared<Font>();
		Ref<SpriteRender> mIndex1;
		WinData mWinData{};
		Animation mAimator;
	private:
		void LoadIconsTexture(std::string filename);
	};
}