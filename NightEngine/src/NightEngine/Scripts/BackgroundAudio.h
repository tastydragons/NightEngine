#pragma once

#include "IScript.h"

#include "GLFW/glfw3.h"

#include "NightEngine/Core/Input.h"

namespace Night
{
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