#include "hzpch.h"

#include "BackgroundAudio.h"
#include "NightEngine/Audio/AudioEngine.h"
#include "NightEngine/ECS/Components/Audio.h"

namespace Night
{
	BackgroundAudio::BackgroundAudio() {}

	BackgroundAudio* BackgroundAudio::Clone() const
	{
		return new BackgroundAudio(*this);
	}

	void BackgroundAudio::Start()
	{
		//check if it has an audio loaded 
		if (p_ecs.HaveComponent<Audio>(GetScriptEntityID()) && (p_ecs.GetComponent<Audio>(GetScriptEntityID()).GetSize() > 0))
		{
			p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].is_Looping = true;
		}
	}

	void BackgroundAudio::Update(float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);
		if (p_ecs.HaveComponent<Audio>(GetScriptEntityID()) && (p_ecs.GetComponent<Audio>(GetScriptEntityID()).GetSize() > 0))
		{
			p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].is_Looping = true;
		}
	}

	void BackgroundAudio::End()
	{
		p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].should_stop = true;
		p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].is_Looping = false;
		delete this;
	}

	std::string BackgroundAudio::GetScriptName()
	{
		return "BackgroundAudio";
	}
}