#pragma once

#include "IComponent.h"

#pragma warning( disable : 26495 )

namespace Night
{
	class Audio : public IComponent
	{
	public:
		enum class AudioType
		{
			NONE,
			MASTER,
			BGM,
			SFX
		};
		struct AudioPiece {
			std::string mAudioPath{};
			AudioType mChannelGroup{};
			int mChannel{};
			bool is_Looping{};
			bool should_play{};
			bool should_stop{};
			bool is_Playing{};
			bool triggered{};
		};
		Audio();
		~Audio() = default;
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		std::vector<AudioPiece>& GetArr() { return AudioArr; }
		void SetArr(std::vector<AudioPiece> in) { AudioArr = in; }
		int GetSize() { return (int)AudioArr.size(); }
		AudioPiece& operator[](int i) { return AudioArr[i]; }

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

		float& GetBGMVolume() { return bgmVol; }
		float& GetSFXVolume() { return sfxVol; }
		float& GetMasterVolume() { return masterVol; }

		void SetBGMVolume(float vol) { bgmVol = vol; }
		void SetSFXVolume(float vol) { sfxVol = vol; }
		void SetMasterVolume(float vol) { masterVol = vol; }
	private:
		std::vector<AudioPiece> AudioArr;
		float bgmVol;
		float sfxVol;
		float masterVol;
	};
}