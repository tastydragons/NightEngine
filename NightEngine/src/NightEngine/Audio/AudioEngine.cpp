#include "hzpch.h"

#include "AudioEngine.h"
#include "NightEngine/ResourceManager/ResourceManager.h"

std::unique_ptr<CAudioEngine> m_Instance;

std::unique_ptr<CAudioEngine>& CAudioEngine::GetInstance()
{
    if (!m_Instance)
    {
        m_Instance = std::make_unique<CAudioEngine>();
    }
    return m_Instance;
}

/*!*************************************************************************
Error check for FMOD audio
****************************************************************************/
void CAudioEngine::ErrorCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK) {
    }
}

/*!*************************************************************************
Load sound function stores the audio selected into a map and calls the FMOD
API create sound
****************************************************************************/
FMOD::Sound* CAudioEngine::Loadsound(const std::string& strSoundName, bool b_Looping)
{
    auto tFoundIt = mSoundMap.find(strSoundName); //find if sound is loaded
    FMOD::Sound* pSound = nullptr; //create sound
    if (tFoundIt != mSoundMap.end())
        return pSound; //if loaded exit function

    FMOD_MODE eMode = FMOD_DEFAULT;
    eMode |= b_Looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

    CAudioEngine::ErrorCheck(mpSystem->createSound(strSoundName.c_str(), eMode, NULL, &pSound));
    if (pSound)
    {
        mSoundMap[strSoundName] = pSound; //if sound is created store in sound map
    }
    return pSound;
}

/*!*************************************************************************
Play sound searches for the audio laoaded into the soumd map, then calls
FMOD API play sound using loaded audio
****************************************************************************/
int CAudioEngine::PlaySound(const std::string& strSoundName, Night::Audio::AudioType chgrp)
{
    auto tFoundIt = mSoundMap.find(strSoundName);
    FMOD::Sound* pSound;

    if (tFoundIt == mSoundMap.end()) //iterate through channels, if not in any, load sound into a channel
    {
        pSound = Loadsound(strSoundName);
    }
    else
    {
        pSound = tFoundIt->second;
    }

    FMOD::Channel* pChannel = nullptr;

    //create new channel if no empty channels found
    int nChannelId = mNextChannelId++;
    ErrorCheck(mpSystem->playSound(pSound, nullptr, false, &pChannel));
    if (pChannel)
    {
        FMOD_MODE currMode;
        pSound->getMode(&currMode);
        mChannelMap[nChannelId] = pChannel;
        //assign channel group to audio
        if (chgrp == Night::Audio::AudioType::MASTER) {
            pChannel->setChannelGroup(Master);
        }
        if (chgrp == Night::Audio::AudioType::BGM) {
            pChannel->setChannelGroup(BGM);
        }
        if (chgrp == Night::Audio::AudioType::SFX) {
            pChannel->setChannelGroup(SFX);
        }

    }
    return nChannelId;
}

/*!*************************************************************************
Uses FMOD setPaused() to pause audio
****************************************************************************/
void CAudioEngine::PauseSound(int channelID)
{
    auto it = mChannelMap.find(channelID);

    if (it != mChannelMap.end())
    {
        it->second->setPaused(true);
    }
}

/*!*************************************************************************
Uses FMOD setPaused() and sets to false to unpause audio
****************************************************************************/
void CAudioEngine::UnpauseSound(int channelID)
{
    auto it = mChannelMap.find(channelID);

    if (it != mChannelMap.end())
    {
        it->second->setPaused(false);
    }
}

/*!*************************************************************************
Stops audio channel groups
****************************************************************************/
void CAudioEngine::StopChannel(channel_groups chan)
{
    if (chan == channel_groups::master)
    {
        Master->stop();
    }

    else if (chan == channel_groups::bgm)
    {
        BGM->stop();
    }

    else if (chan == channel_groups::sfx)
    {
        SFX->stop();
    }
}

/*!*************************************************************************
Sets audio volume
****************************************************************************/
void CAudioEngine::SetVolume(int channelID, float vol)
{
    auto it = mChannelMap.find(channelID);

    if (it != mChannelMap.end())
    {
        it->second->setVolume(VolumeTodB(vol));
    }
}

/*!*************************************************************************
Stop all audio channels
****************************************************************************/
void CAudioEngine::StopChannel(int channelID)
{
    auto it = mChannelMap.find(channelID);

    if (it != mChannelMap.end())
    {
        it->second->stop();
    }
}

/*!*************************************************************************
Returns the current volume level
****************************************************************************/
float CAudioEngine::GetVolume(int channelID)
{
    float audio_volume = 0.0f;

    auto it = mChannelMap.find(channelID);

    if (it != mChannelMap.end())
    {
        it->second->getVolume(&audio_volume);
    }
    return audio_volume;

}

/*!*************************************************************************
Init loop of CAudioEngine
****************************************************************************/
void CAudioEngine::Init()
{
    ErrorCheck(FMOD::System_Create(&mpSystem));

    mpSystem->init(1024, FMOD_INIT_NORMAL, NULL);
    mpSystem->createChannelGroup("Master", &Master);
    mpSystem->createChannelGroup("BGM", &BGM);
    mpSystem->createChannelGroup("SFX", &SFX);

    Master->addGroup(BGM);
    Master->addGroup(SFX);

    //load all audio files from metadigger folder
    std::string audio_path = "Assets/metadigger/";
    for (auto const& dir_entry : std::filesystem::directory_iterator{ audio_path }) //iterate files in metadigger file
    {
        //checks if given file status or path corresponds to a regular file
        if (!dir_entry.is_regular_file())
        {
            continue;
        }
    }
}

/*!*************************************************************************
Update loop of CAudioEngine
****************************************************************************/
void CAudioEngine::Update()
{
    std::vector<std::map<int, FMOD::Channel*>::iterator> pStoppedChannels;
    for (auto it = mChannelMap.begin(); it != mChannelMap.end(); ++it)
    {
        bool bIsPlaying = false;
        it->second->isPlaying(&bIsPlaying);

        if (!bIsPlaying)
        {
            pStoppedChannels.push_back(it);
        }
    }
    for (auto& it : pStoppedChannels)
    {
        mChannelMap.erase(it);
    }
    if (mChannelMap.begin() == mChannelMap.end()) {
        mNextChannelId = 0;
    }
    CAudioEngine::ErrorCheck(mpSystem->update());
}


/*!*************************************************************************
Release loop of CAudioEngine, releases all audio files
****************************************************************************/
void CAudioEngine::Release()
{
    // iterate through mSoundMap
    // release every sound
    for (auto sound_it = mSoundMap.begin(); sound_it != mSoundMap.end(); sound_it++)
    {
        sound_it->second->release();
    }

    // release every channel group
    // release systems
    BGM->stop();
    SFX->stop();
    Master->stop();

    BGM->release();
    SFX->release();
    Master->release();

    mpSystem->release();
}

/*!*************************************************************************
Checks if audio is playing
****************************************************************************/
bool CAudioEngine::IsPlaying(int nChannelId) const
{
    bool is_playing = false;
    auto it = mChannelMap.find(nChannelId);
    //if not found return false
    if (it != mChannelMap.end())
    {
        it->second->isPlaying(&is_playing);
        return is_playing;
    }
    return false;
}
/*!*************************************************************************
Converts decibels to volume
****************************************************************************/
float  CAudioEngine::dbToVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

/*!*************************************************************************
Converts volume to decibels
****************************************************************************/
float  CAudioEngine::VolumeTodB(float volume)
{
    float dec = 20.0f * log10f(volume);

    if (dec > 150.0f)
    {
        //Set cap if volume is maxed
        dec = 149.f;
    }

    return dec;
}


void CAudioEngine::SetLooping(const std::string& strSoundName, bool in) {
    if (in) {
        auto tFoundIt = mSoundMap.find(strSoundName);
        if (tFoundIt != mSoundMap.end()) {
            tFoundIt->second->setMode(FMOD_LOOP_NORMAL);
            tFoundIt->second->setLoopCount(-1);
        }
    }
    else {
        auto tFoundIt = mSoundMap.find(strSoundName);
        if (tFoundIt != mSoundMap.end()) {
            tFoundIt->second->setMode(FMOD_LOOP_OFF);
        }
    }
}

float CAudioEngine::GetVolumeByChannel(FMOD::ChannelGroup* ch) {
    float ret = 0;
    ch->getVolume(&ret);
    return ret;
}