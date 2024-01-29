#pragma once

#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#include "fmod.hpp"
#include "NightEngine/ECS/Components/Audio.h"

#define p_Audio CAudioEngine::GetInstance()

class CAudioEngine //Handles loading, unloading, playing, stopping and changing sounds
{

    FMOD::System* mpSystem;

    int mNextChannelId = 1;

    std::map<std::string, FMOD::Sound*> mSoundMap;


    FMOD::ChannelGroup* BGM, * SFX, * Master;
    FMOD::Sound* Msound;
public:
    std::map<int, FMOD::Channel*> mChannelMap;

    enum class channel_groups { master, bgm, sfx };
    static std::unique_ptr<CAudioEngine>& GetInstance();

    void Init();
    void Update();
    void Release();
    void ErrorCheck(FMOD_RESULT result);

    FMOD::Sound* Loadsound(const std::string& strSoundName, bool bLooping = false);

    int PlaySound(const std::string& strSoundName, Night::Audio::AudioType chgrp);
    void PauseSound(int channelID);
    void UnpauseSound(int channelID);
    void StopChannel(channel_groups chan);
    void SetVolume(int channelID, float vol);
    void StopChannel(int channelID);
    float GetVolume(int channelID);
    void SetLooping(const std::string& strSoundName, bool in);
    bool IsPlaying(int nChannelId) const;
    float dbToVolume(float db);
    float VolumeTodB(float volume);
    int GetPlayingChannels() { return (int)mChannelMap.size(); }
    void SetVolumeByChannel(FMOD::ChannelGroup* ch, float volume) { ch->setVolume(volume); }
    float GetVolumeByChannel(FMOD::ChannelGroup* ch);
    FMOD::ChannelGroup* GetMasterChannelGroup() { return Master; }
    FMOD::ChannelGroup* GetBGMChannelGroup() { return BGM; }
    FMOD::ChannelGroup* GetSFXChannelGroup() { return SFX; }
};

#endif // !_AUDIO_ENGINE_H_