/*!*************************************************************************
****
\file BackgroundAudio.h
\author Cheung Jun Yin Matthew
\par DP email: j.cheung@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 28/01/2023
\brief	Script for BGM

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#pragma once
#include "IScript.h"
#include "GLFW/glfw3.h"
#include "ExoEngine/Input/Input.h"

namespace EM {
	class BackgroundAudio : public IScript
	{
	public:
		BackgroundAudio();
		~BackgroundAudio() = default;
		virtual BackgroundAudio* Clone() const override;
		virtual void Start() override;
		virtual void Update(float Frametime) override;
		virtual void End() override;
		virtual std::string GetScriptName();
		virtual void SetScriptEntityID(Entity& entity) override { entityID = entity; }
		virtual Entity& GetScriptEntityID() override { return entityID; }

	private:
		
	};
}