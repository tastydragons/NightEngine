#include "hzpch.h"

#include "NightEngine/ECS/Components/Audio.h"

/*!*************************************************************************
Constructor for audio component
****************************************************************************/
Night::Audio::Audio() : AudioArr(), bgmVol{ 1.0f }, sfxVol{ 1.0f }, masterVol{ 1.0f } {}


/*!*************************************************************************
Deserialize audio component
****************************************************************************/
bool Night::Audio::Deserialize(const rapidjson::Value& obj)
{
	for (int i = 0; i < obj["AudioArrCount"].GetInt(); ++i)
	{
		AudioPiece na{};
		na.mAudioPath = obj[std::to_string(i).c_str()].GetObj()["AudioPath"].GetString();
		na.mChannelGroup = static_cast<AudioType>(obj[std::to_string(i).c_str()].GetObj()["Channel"].GetInt());
		na.is_Looping = obj[std::to_string(i).c_str()].GetObj()["IsLooping"].GetBool();
		na.mChannel = 0;
		na.should_play = false;
		na.should_stop = false;
		na.is_Playing = false;
		na.triggered = false;
		AudioArr.emplace_back(na);
	}
	bgmVol = (float)obj["BGM Volume"].GetDouble();
	sfxVol = (float)obj["SFX Volume"].GetDouble();
	masterVol = (float)obj["Master Volume"].GetDouble();
	return true;
}

/*!*************************************************************************
Serialise audio component
****************************************************************************/
bool Night::Audio::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
{
	int i = 0;
	for (i = 0; i < AudioArr.size(); ++i)
	{
		writer->Key(std::to_string(i).c_str());
		writer->StartObject();
		writer->Key("AudioPath");
		writer->String(AudioArr[i].mAudioPath.c_str());
		writer->Key("Channel");
		writer->Int(static_cast<int>(AudioArr[i].mChannelGroup));
		writer->Key("IsLooping");
		writer->Bool(AudioArr[i].is_Looping);
		writer->EndObject();
	}
	writer->Key("AudioArrCount");
	writer->Int(i);
	writer->Key("BGM Volume");
	writer->Double(bgmVol);
	writer->Key("SFX Volume");
	writer->Double(sfxVol);
	writer->Key("Master Volume");
	writer->Double(masterVol);
	return true;
}