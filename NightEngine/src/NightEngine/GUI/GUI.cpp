#include "hzpch.h"

#include "GUI.h"

#include "NightEngine/Core/Application.h"
#include "NightEngine/Core/Input.h"
#include "NightEngine/Renderer/Graphics.h"
#include "NightEngine/ECS/SceneManager.h"

#include "Platform/Windows/Window.h"

namespace Night
{
	std::unique_ptr<gui_system> mInstance; //create a unique pointer to be used outside of this file
	/*!*************************************************************************
	Create the instance for Class GUI
	****************************************************************************/
	std::unique_ptr<gui_system>& gui_system::GetInstance()// in the even that no unique pointer is created, create one 
	{
		if (!mInstance)// check if the unique pointer is created
		{
			mInstance = std::make_unique<gui_system>();// create it, if it doesnt exist
		}
		return mInstance;//pass it back
	}
	/*!*************************************************************************
   Check the mouse cursor is in the bounding box
   ****************************************************************************/
	bool gui_system::is_within_box(glm::vec2 cur, button_bb box)
	{
		//We factor in aspect ratio since graphic scale is proportionate to aspect ratio instead of screen
		if ((cur.x * mAspectRatio > box.min.x && cur.y > box.min.y) && (cur.x * mAspectRatio < box.max.x && cur.y < box.max.y))
			return true;//return true if the position of the cursor is between both of the button minimun and maximum bounding point
		else
			return false;// return false if it isnt
	}
	/*!*************************************************************************
   Set the bounding box for the button
   ****************************************************************************/
	void gui_system::set_pause_button(vec2D pos, float scaleX, float scaleY)
	{
		mPauseButton.max.y = pos.y + scaleY / 2;//create the upper Y coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mPauseButton.max.x = (pos.x + (scaleX / 2));//create the upper X coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mPauseButton.min.y = pos.y - scaleY / 2;//create the lower Y coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mPauseButton.min.x = (pos.x - (scaleX / 2));//create the lower X coordinate bounding box by dividing the scale by 2 and adding it to the center position



	}
	/*!*************************************************************************
   Set the bounding box for the continue button
   ****************************************************************************/
	void gui_system::set_continue_button(vec2D pos, float scaleX, float scaleY)
	{
		mContinueButton.max.y = pos.y + scaleY / 2;//create the upper Y coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mContinueButton.max.x = (pos.x + (scaleX / 2));//create the upper X coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mContinueButton.min.y = pos.y - scaleY / 2;//create the lower Y coordinate bounding box by dividing the scale by 2 and adding it to the center position
		mContinueButton.min.x = (pos.x - (scaleX / 2));//create the lower X coordinate bounding box by dividing the scale by 2 and adding it to the center position
	}
	/*!*************************************************************************
   Update the GUI system
   ****************************************************************************/
	bool  gui_system::Update(Window* screen)
	{

		glm::vec2 mouse_pos{ static_cast<float>(screen->Getter().mouseX) , static_cast<float>(screen->Getter().mouseY) };//store the mouse position into a vector
		glm::vec2 screen_size{ static_cast<float>(screen->Getter().m_Width),static_cast<float>(screen->Getter().m_Height) };//store the screen size into a vector
		mAspectRatio = screen_size.x / screen_size.y;//find and store the aspect ratio of the screen



		//function to convert mouse to screen position
		mouse_pos.x /= screen_size.x;
		mouse_pos.y /= screen_size.y;
		mouse_pos *= 2.0f;
		mouse_pos -= glm::vec2{ 1, 1 };
		mouse_pos.y *= -1;

		MousePosition = { mouse_pos.x * mAspectRatio * 1 / Graphic::camera.GetZoomLevel() , mouse_pos.y };
		if (is_Pause == true && is_menu == false)//check if the system is pause
		{
			if (is_within_box(mouse_pos, mPauseButton) && p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT))//if system is pause and the quit button is pressed, tell the system to quit the game
			{
				return true;
			}

			if (is_within_box(mouse_pos, mContinueButton) && p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT))//if system is pause and continue button is pressed, tell the system to resume the game
			{
				is_Pause = false;
			}
			else return false;

		}
		if (Script_End == true)
		{
			return true;
		}
		return false;
	}


	/*!*************************************************************************
	Check pause states
	****************************************************************************/
	bool gui_system::check_pause()//getter function for pause state
	{
		return is_Pause;
	}
	bool gui_system::Check_script()
	{
		return Script_End;
	}
	bool gui_system::Check_menu()
	{
		return is_menu;
	}
	bool gui_system::Check_HTP()
	{
		return HTP;
	}
	/*!*************************************************************************
	Pause states toggling
	****************************************************************************/
	void gui_system::toggle_pause()//if game is pause,unpause it. Or pause it if it is unpause.
	{
		if (is_Pause == false)
		{
			is_Pause = true;
			p_Scene->isGameplay = false;
		}
		else
		{
			is_Pause = false;
			p_Scene->isGameplay = true;
		}
	}
}