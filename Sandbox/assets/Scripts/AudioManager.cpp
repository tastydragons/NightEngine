/*!*************************************************************************
****
\file AudioManager.cpp
\author Tan Ek Hern
\par DP email: t.ekhern@digipen.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM200
\date 28/01/2023
\brief	Script for Audio Manager

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/
#include "empch.h"
#include "AudioManager.h"
#include "ExoEngine/Audio/AudioEngine.h"

namespace EM
{
    /*!*************************************************************************
    Default constructor for Audio Manager
    ****************************************************************************/
    AudioManager::AudioManager() : BGMvol { 20.f }, SFXvol { 50.f }, Mastervol { 50.f } {}
    /*!*************************************************************************
    Returns a new copy of AudioManager Script
    ****************************************************************************/
	AudioManager* AudioManager::Clone() const
	{
		return new AudioManager(*this);
	}
    /*!*************************************************************************
    Start State of AudioManager Script
    ****************************************************************************/
	void AudioManager::Start()
	{
        if (p_ecs.HaveComponent<Audio>(GetScriptEntityID())) 
        {
            auto& audio = p_ecs.GetComponent<Audio>(GetScriptEntityID());
            for (int i = 0; i < audio.GetSize(); i++) {
                p_Audio->SetLooping(audio[i].mAudioPath, audio[i].is_Looping);
            }
        }
	}
    /*!*************************************************************************
    Update Loop of AudioManager Script
    ****************************************************************************/
	void AudioManager::Update(float Frametime)
	{
        UNREFERENCED_PARAMETER(Frametime);
        p_Audio->Update();
        //update volume values
        //check sounds and update
        if (p_ecs.HaveComponent<Audio>(GetScriptEntityID())) {
            auto& audio = p_ecs.GetComponent<Audio>(GetScriptEntityID());
            for (int i = 0; i < audio.GetSize(); i++) {
                //check if sound is playing
                if (audio[i].triggered)
                {
                    audio[i].is_Playing = p_Audio->IsPlaying(audio[i].mChannel);
                    if (audio[i].is_Playing == false) {
                        audio[i].triggered = false;
                    }
                }
                if (audio[i].is_Looping == false && audio[i].should_play == true && audio[i].is_Playing == false) {
                    //play sound
                    audio[i].mChannel = p_Audio->PlaySound("Assets/Metadigger/" + audio[i].mAudioPath, audio[i].mChannelGroup);
                    audio[i].should_play = false;
                    audio[i].triggered = true;
                }
                if (audio[i].is_Looping == true && audio[i].is_Playing == false) {
                    //play sound
                    audio[i].mChannel = p_Audio->PlaySound("Assets/Metadigger/" + audio[i].mAudioPath, audio[i].mChannelGroup);
                    audio[i].triggered = true;
                }
                if (audio[i].should_stop == true) {
                    p_Audio->StopChannel(audio[i].mChannel);
                    audio[i].is_Playing = false;
                    audio[i].triggered = false;
                    audio[i].should_stop = false;
                }
                audio[i].should_play = false;
            }
        }
	}
    /*!*************************************************************************
    End State for Audio Manager
    ****************************************************************************/
    void AudioManager::End()
    {
        delete this;
    }

    std::string AudioManager::GetScriptName()
    {
        return "AudioManager";
    }
}