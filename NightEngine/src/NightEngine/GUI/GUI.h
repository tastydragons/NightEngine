#pragma once

#include "NightEngine/Core/Input.h"
#include "NightEngine/Math/NEMath.h"

#include "Platform/System/System.h"
#include "Platform/Windows/Window.h"

#include "NightEngine/Core/Core.h"

#define p_GUI Night::gui_system::GetInstance()

namespace Night
{
	class NIGHT_API gui_system
	{
	private:
		bool is_Pause = { false }; //a boolean to see if the system is currently pause
		bool Script_End = { false };// a boolean that check if the end for the game loop is called via the script
		bool is_menu = { true }; // a boolean to check if the menu is currently open
		bool HTP = { false }; // a boolean to check if how to play on the main menu is on


		struct button_bb // a pseudo bounding box not tied to collision meant to check if the cusor is touching the button
		{
			glm::vec2 max{ 0,0 };//top right of the button
			glm::vec2 min{ 0,0 };//buttom left of thr button
		};

		button_bb mPauseButton{}; //bounding box of the pause button (to be replace with factory)
		button_bb mContinueButton{};//bounding box of the continue button (to be replace with factory)
		float mAspectRatio{}; //hold local value of aspecrt ratio

	public:
		static std::unique_ptr<gui_system>& GetInstance(); //used to create an instance to work alongside the game during it's lifetime
		glm::vec3 PrevCamPos;
		bool Update(Window* screen); // to be update in realtime  with the rest of the engine system
		bool mPauseSwitch = false; // a secondary boolean to prevent button from flickering 
		glm::mat4 VPmat{}; //hold the viewprojection matrix passed from the camera
		bool sub_menu{ false };
		void set_pause_button(vec2D pos, float scaleX, float scaleY); //set the position and scale of the pause button
		void set_continue_button(vec2D pos, float scaleX, float scaleY);//set the position and scale of the continue button
		float Get_AspectRatio() { return mAspectRatio; }//set the position and scale of the continue button
		void toggle_pause();// used to toggle the is_pause boolean
		void toggle_script() { Script_End = Script_End ? false : true; }//to be used by script
		void toggle_menu() { is_menu = is_menu ? false : true; }
		void toggle_HTP() { HTP = HTP ? false : true; }
		bool check_pause();// a getter function for the pause
		bool Check_script(); //check if the qquit is called by the script
		bool Check_menu(); // check if the menu is on 
		bool Check_HTP(); // check if player is currently in how to play in the main menu
		bool is_within_box(glm::vec2 cur, button_bb box);// a helper function to check  if the cursor is touching the button,
		glm::vec2 MousePosition{};
	};
}