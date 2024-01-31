/*!*************************************************************************
****
\file PauseMenu.cpp
\author Lau Yong Hui
\par DP email:l.yonghui.edu
\par Course: CSD2400
\par Section: a
\par Assignment GAM250
\date 12/02/2023
\brief	Script that allow pause menu related User interface to used when the game is set to a pause state

Copyright (C) 20xx DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of Technology is prohibited.
****************************************************************************
***/

#include "empch.h"
#include "PauseMenu.h"
#include "../ECS/SceneManager.h"
#include "ExoEngine/GUI/GUI.h"
#include "Platform/Graphics/Graphics.h"
#include "Platform/LevelEditor/LevelEditor.h"

namespace EM
{
	/*!*************************************************************************
	Start state for Pause Menu
	****************************************************************************/
	void PauseMenu::Start()
	{
	}
	/*!*************************************************************************
	Update state for Pause Menu
	****************************************************************************/
	void PauseMenu::Update(float Frametime)
	{

		UNREFERENCED_PARAMETER(Frametime);
		//this set of codes are to allow easy access to the game component when coding
		auto& transform = p_ecs.GetComponent<Transform>(GetScriptEntityID());
		auto& ID_tag = p_ecs.GetComponent<Tag>(GetScriptEntityID());
		auto& col = p_ecs.GetComponent<Collider>(GetScriptEntityID());
		auto& spt = p_ecs.GetComponent<Sprite>(GetScriptEntityID());


		vec2D camPos = vec2D(Graphic::mcamera->GetPosition().x, Graphic::mcamera->GetPosition().y);
		//obtain the camera position from graphic


		if (p_Input->KeyPressed(GLFW_KEY_ESCAPE)) // check if the game is set to a pause state
		{
			if (ID_tag.GetTag() == "PauseMenuBG") // only entity with "PauseMenuBG"  can toggle the pause taste
			{
				if (p_GUI->sub_menu == true) //this code is to allow the camera to return to the pause menu when escape is press when the user is accessing one of the sub menu
				{
					Graphic::camera.SetPosition({ 0.0f, -4.0f, 0.0f }); //set position to pause menu
					p_GUI->sub_menu = false;
				}
				else
				{
					p_GUI->toggle_pause();
					Graphic::camera.SetPosition({ 0.0f, -4.0f, 0.0f });
					Graphic::mcamera->SetZoomLevel(1.0f);
				}
			}

		}

		//////////code to set a sprite when the game is set into a pause state/////////////////////////////////////
		if (p_GUI->check_pause() == true && p_Editor->is_ShowWindow == false)
		{

			//////////code to set idle sprite when the game is set into a pause state/////////////////////////////////////
			if (ID_tag.GetTag() == "PauseMenuBG")
			{
				spt.SetTexture("PAUSED");
			}
			if (ID_tag.GetTag() == "Resume")
			{
				spt.SetTexture("ResumeGame_Yellow");

			}
			if (ID_tag.GetTag() == "Quit")
			{
				spt.SetTexture("ExitToMainMenu_Yellow");

			}
			if (ID_tag.GetTag() == "Options")
			{
				spt.SetTexture("Option_Yellow");

			}
			if (ID_tag.GetTag() == "HTP")
			{
				spt.SetTexture("HTP_Yellow");

			}


			if (ID_tag.GetTag() == "Back")
			{
				spt.SetTexture("BACK_Y");
			}

			if (ID_tag.GetTag() == "No")
			{
				spt.SetTexture("NO_Y");
			}

			if (ID_tag.GetTag() == "Yes")
			{
				spt.SetTexture("YES_Y");
			}

			//////////code to set idle sprite when the game is set into a pause state/////////////////////////////////////



				//this code is to check the current state of the mouse
			if (curr_state != button_state::click)
				curr_state = button_state::idle;// return to idle whe the state is set to release at the start of the next game loop


			if (p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT) && curr_state == button_state::idle)
			{
				curr_state = button_state::click; //if left mouse is held down set mouse state to click
			}

			else if (p_Input->MouseIsReleased(GLFW_MOUSE_BUTTON_LEFT) && curr_state == button_state::click)
			{
				curr_state = button_state::release;//when mouse key is release set state to release
			}

			glm::vec2 newmousepos{ (p_GUI->MousePosition.x + camPos.x),(p_GUI->MousePosition.y + camPos.y) };//this code is to check the mouse position in accordance to the camera position

			if (is_within_box(newmousepos, col, transform))//check if the system is hovering over an object, change to the hover sprite if it is
			{
				if (ID_tag.GetTag() == "Resume" && !(p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT)))
				{
					spt.SetTexture("ResumeGame_Blue");
				}
				if (ID_tag.GetTag() == "Quit" && !(p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT)))
				{
					spt.SetTexture("ExitToMainMenu_Blue");
				}
				if (ID_tag.GetTag() == "Options" && !(p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT)))
				{
					spt.SetTexture("Option_Blue"); //to be updated
				}
				if (ID_tag.GetTag() == "HTP" && !(p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT)))
				{
					spt.SetTexture("HTP_Blue"); //to be updated
				}

				if (ID_tag.GetTag() == "Back")
				{
					spt.SetTexture("BACK_B");//to be change
				}

				if (ID_tag.GetTag() == "No")
				{
					spt.SetTexture("NO_B");//to be change
				}

				if (ID_tag.GetTag() == "Yes")
				{
					spt.SetTexture("YES_B");//to be change
				}

				if (curr_state == button_state::release) // when mouse button is release, call the button functions, release is selected to prevent function from calling when mouse is held down 
				{
					if (ID_tag.GetTag() == "Resume")
					{
						p_GUI->toggle_pause(); //change the pause state back when clicked
					}

					if (ID_tag.GetTag() == "Quit")
					{
						Graphic::camera.SetPosition({ 0.0f, 8.0f, 0.0f }); //go to are you sure before quiting
						p_GUI->sub_menu = true;//let the game know that sub menu is being accessed
					}

					if (ID_tag.GetTag() == "Options")
					{
						Graphic::camera.SetPosition({ 0.0f, 4.0f, 0.0f });// go to the option screen
						p_GUI->sub_menu = true;//let the game know that sub menu is being accessed
					}


					if (ID_tag.GetTag() == "Back")
					{
						Graphic::camera.SetPosition({ 0.0f, -4.0f, 0.0f });//return to pause menu
					}

					if (ID_tag.GetTag() == "No")
					{
						Graphic::camera.SetPosition({ 0.0f, -4.0f, 0.0f });//return to pause menu
					}

					if (ID_tag.GetTag() == "Yes")
					{
						p_GUI->toggle_pause();//set pause state back
						p_Scene->setSceneToLoad("Assets/Scene/Menu.json");// return to main menu
					}


					if (ID_tag.GetTag() == "HTP")
					{
						Graphic::camera.SetPosition({ 0.0f, -8.0f, 0.0f });//set camera to how to play
						p_GUI->sub_menu = true;//let the game know that sub menu is being accessed
					}
				}

				//when mouse is held down set the coresspoding sprites to clicked state
				if (curr_state == button_state::click)
				{
					if (ID_tag.GetTag() == "Resume")
					{
						spt.SetTexture("ResumeGame_Red");
					}

					if (ID_tag.GetTag() == "Quit")
					{
						spt.SetTexture("ExitToMainMenu_Red");
					}

					if (ID_tag.GetTag() == "Back")
					{
						spt.SetTexture("BACK_R");
					}

					if (ID_tag.GetTag() == "No")
					{
						spt.SetTexture("NO_R");
					}

					if (ID_tag.GetTag() == "Yes")
					{
						spt.SetTexture("YES_R");
					}

					if (ID_tag.GetTag() == "Options")
					{
						spt.SetTexture("Option_Red");
					}

					if (ID_tag.GetTag() == "HTP")
					{
						spt.SetTexture("HTP_Red");
					}


					if (ID_tag.GetTag() == "PAUSED")
					{
						spt.SetTexture("Restart_Click");
					}

				}
			}
		}


		//////////code to set all entity to when the game is set out of pause state/////////////////////////////////////
		else if (p_GUI->check_pause() == false)
		{
			spt.SetTexture("Blank");
		}

	}
	/*!*************************************************************************
	End state for Pause Menu
	****************************************************************************/
	void PauseMenu::End()
	{
		delete this;
	}
	/*!*************************************************************************
	Copy constructor for Pause Menu
	****************************************************************************/
	PauseMenu* PauseMenu::Clone() const
	{
		return new PauseMenu(*this);
	}
	/*!*************************************************************************
	Get name of Pause Menu
	****************************************************************************/
	std::string PauseMenu::GetScriptName()
	{
		return "PauseMenu";
	}
	/*!*************************************************************************
	Check if cursor is within the bounds of the box
	****************************************************************************/
	bool PauseMenu::is_within_box(glm::vec2 cur, Collider box, Transform trans) //unique aabb code for entity and mouse
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