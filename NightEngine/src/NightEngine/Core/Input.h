#pragma once

#include "NightEngine/Core/Core.h"

#define p_Input Night::Input::GetInstance()


namespace Night
{
	template class NIGHT_API std::vector <int, std::allocator<int>>;
	template class NIGHT_API std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, std::allocator<std::pair<const int, int>>>;

	using keystatus = int;
	using keycode = int;
	using mousestatus = int;
	using mousecode = int;

	class NIGHT_API Input
	{
	public:
		//Pointer to input system
		static std::unique_ptr<Input>& GetInstance();

		// key inputs 
		bool KeyPressed(const int& key);
		bool KeyHold(const int& key);
		bool KeyReleased(const int& key);

		void SetKeyStatus(keycode key, keystatus status);
		void ResetPressedKey();

		//mouse inputs
		bool MousePressed(const int& key);
		bool MouseHold(const int& key);
		bool MouseIsReleased(const int& key);

		void SetMouseStatus(mousecode key, mousestatus status);
		void ResetPressedMouse();

		bool isKeyPressed(keycode key);

	public:
		//data
		std::unordered_map<keycode, keystatus> mKeyStatus;
		std::unordered_map<mousecode, mousestatus> mMouseStatus;
		std::vector<keycode> mReset;
		std::vector<mousecode> mouseReset;

		double mMouseScrollStatus = 0.0f;

	};
	};
}