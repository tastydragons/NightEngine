#include "hzpch.h"
#include "SliderScript.h"
#include "../ECS/SceneManager.h"
#include "NightEngine/GUI/GUI.h"
#include "Platform/LevelEditor/LevelEditor.h"
#include "NightEngine/Renderer/Graphics.h"

namespace Night
{
	/*!*************************************************************************
	Start State of Slider Script
	****************************************************************************/
	void SliderScript::Start()
	{
		clicked = false;//set click to false at the start
		selected = false;// set select to false at the start

		def_position = p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos();//set defualt position at the start
		limit.min = { def_position.x - 0.5f,def_position.y - 0.1f }; // set minium volume limit at the start
		limit.max = { def_position.x + 0.5f,def_position.y + 0.1f }; // set maximum volume limit at the start
	}
	/*!*************************************************************************
	Update State of Slider Script
	****************************************************************************/
	void SliderScript::Update(float Frametime)
	{
		UNREFERENCED_PARAMETER(Frametime);

		vec2D camPos = vec2D(Graphic::mcamera->GetPosition().x, Graphic::mcamera->GetPosition().y); // set cameraposition

		if (p_Editor->is_ShowWindow == true || limitset == false) // this code allow the slider position and limit to  be edited in the editor
		{
			if (SliderCali == true)
			{
				p_ecs.GetComponent<Transform>(GetScriptEntityID()).SetPos(limit.min.x + 0.5f, p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos().y);
				SliderCali = false;//slider have been calibrated
			}
			def_position = p_ecs.GetComponent<Transform>(GetScriptEntityID()).GetPos();
			limit.min = { def_position.x - 0.5f,def_position.y - 0.1f }; // calculate the minimum position
			limit.max = { def_position.x + 0.5f,def_position.y + 0.1f };// calculate the maximum position
			limitset = true;// let the entity know that limit have been set
		}
		//this set of codes are to allow easy access to the game component when coding
		auto& transform = p_ecs.GetComponent<Transform>(GetScriptEntityID());
		auto& ID_tag = p_ecs.GetComponent<Tag>(GetScriptEntityID());

		if (p_Editor->is_ShowWindow == false) // when not in editor mode set the slider position in accordance to the current volume set
		{
			if (ID_tag.GetTargetTag() == "Master")
				transform.SetPos(p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) + limit.min.x, transform.GetPos().y);

			if (ID_tag.GetTargetTag() == "BGM")
				transform.SetPos(p_Audio->GetVolumeByChannel(p_Audio->GetBGMChannelGroup()) + limit.min.x, transform.GetPos().y);

			if (ID_tag.GetTargetTag() == "SFX")
				transform.SetPos(p_Audio->GetVolumeByChannel(p_Audio->GetSFXChannelGroup()) + limit.min.x, transform.GetPos().y);

			SliderCali = true;
			if ((p_GUI->MousePosition.x + camPos.x >= limit.min.x && p_GUI->MousePosition.y + camPos.y >= limit.min.y) &&
				(p_GUI->MousePosition.x + camPos.x <= limit.max.x && p_GUI->MousePosition.y + +camPos.y <= limit.max.y)) // check if the mouse is inside the slider 
			{
				if (p_Input->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) // when mouse is held down, allow the volume ball to be adjusted
				{
					clicked = true;
					transform.SetPos(p_GUI->MousePosition.x, transform.GetPos().y);
					if (p_ecs.HaveComponent<Audio>(GetScriptEntityID()) && ((p_ecs.GetComponent<Audio>(GetScriptEntityID())).GetSize() > 0))
					{
						p_ecs.GetComponent<Audio>(GetScriptEntityID())[0].should_play = true; //volume slider click audio
					}
				}
				else
				{
					clicked = false;// let the entity know that it is not being clicked on
				}
			}


			if (ID_tag.GetTargetTag() == "Master")// when master is  being adjusted
			{
				p_Audio->SetVolumeByChannel(p_Audio->GetMasterChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x)); //calculate and set the volume of master
				if (p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) < 0.01)
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetMasterChannelGroup(), 0.0f); // if it is less than 0.01, set sound to mute
				}
				if (p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) < p_Audio->GetVolumeByChannel(p_Audio->GetSFXChannelGroup())) //adjust SFX Volume to ensure volume is equal or less than master at all time
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetSFXChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));
				}

				if (p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) < p_Audio->GetVolumeByChannel(p_Audio->GetBGMChannelGroup()))//adjust BGM Volume to ensure volume is equal or less than master at all time
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetBGMChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));
				}

			}

			if (ID_tag.GetTargetTag() == "BGM")
			{
				p_Audio->SetVolumeByChannel(p_Audio->GetBGMChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));//calculate and set the volume of BGM
				if (p_Audio->GetVolumeByChannel(p_Audio->GetBGMChannelGroup()) < 0.01) // if it is less than 0.01, set sound to mute
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetBGMChannelGroup(), 0.0f);
				}

				if (p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) < p_Audio->GetVolumeByChannel(p_Audio->GetBGMChannelGroup()))
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetMasterChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));//adjust Master volume when BGM volume is larger than Master
				}
			}
			if (ID_tag.GetTargetTag() == "SFX")
			{
				p_Audio->SetVolumeByChannel(p_Audio->GetSFXChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));//calculate and set the volume of BGM
				if (p_Audio->GetVolumeByChannel(p_Audio->GetSFXChannelGroup()) < 0.01) // if it is less than 0.01, set sound to mute
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetSFXChannelGroup(), 0.0f);
				}

				if (p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup()) < p_Audio->GetVolumeByChannel(p_Audio->GetSFXChannelGroup()))
				{
					p_Audio->SetVolumeByChannel(p_Audio->GetMasterChannelGroup(), transform.GetPos().x - limit.min.x / (limit.max.x - limit.min.x));//adjust Master volume when SFX volume is larger than Master
				}
			}
		}
	}
	/*!*************************************************************************
	End State of Slider Script
	****************************************************************************/
	void SliderScript::End()
	{
		delete this;
	}
	/*!*************************************************************************
	Clones the Slider Script
	****************************************************************************/
	SliderScript* SliderScript::Clone() const
	{
		return new SliderScript(*this);
	}

	/*!*************************************************************************
	Returns the names of the Slider Script
	****************************************************************************/
	std::string SliderScript::GetScriptName()
	{
		return "SliderScript";
	}
	/*!*************************************************************************
	Checks if slider is within the slider box
	****************************************************************************/
	bool SliderScript::is_within_box(glm::vec2 cur, Collider box, Transform trans)
	{
		if (cur.x > ((box[0].mMin.x * (trans.GetScale().x / 2)) + trans.GetPos().x) && cur.y > (box[0].mMin.y * (trans.GetScale().y / 2) + trans.GetPos().y) &&
			cur.x < ((box[0].mMax.x * (trans.GetScale().x / 2)) + trans.GetPos().x) && cur.y < (box[0].mMax.y * (trans.GetScale().y / 2) + trans.GetPos().y))
		{
			return true;//return true if the position of the cursor is between both of the button minimun and maximum bounding point
		}
		else
			return false;// return false if it isnt
	}
}