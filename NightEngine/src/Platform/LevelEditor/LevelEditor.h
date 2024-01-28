#pragma once

#include "Platform/Windows/Window.h"
#include "NightEngine/ResourceManager/ResourceManager.h"

#define p_Editor Night::LevelEditor::GetInstance()

namespace Night
{
	class LevelEditor
	{
	public:
		//ctor and dtor
		LevelEditor() = default;
		~LevelEditor() = default;

		//main function for running
		void Init(Window* window);
		void LoadSceneFromFile(); //Load all the json file into a container for easy access in imgui
		void Update();	//update loop
		void Draw();
		void End();

		void LoadSaveScene();
		void SceneViewer();
		void ContentBrowser();
		void Logger();
		void Profiler();
		void Hierarchy();
		void Inspector();

		//for audio loading from files
		void LoadAudioFromFile();
		void LoadScriptsFromFile();
		void AudioEditor();
		void insertAudioFilePath(std::string in);
		void insertTextureFilePath(std::string on);

		//for textures loading from files
		void LoadTextureFromFile();

		Window* GetWindow() { return mWindow; }


	public:

		bool is_ShowWindow = true;

		static std::unique_ptr<LevelEditor>& GetInstance();


		//inspector and Hierarchy
		Entity selectedEntity = {};


		//for inspector and Hierarchy
		bool mDebugDraw{ false }; //to show debug/collision draw 

		//For resize and sceneviewer/viewport
		ImVec2 mViewportSize = { 0.0f, 0.0f };
		ImVec2 mViewportBounds[2];
		bool mViewportFocused = false;
		ImVec2 mGameMousePosition = { 0.0f, 0.0f };
		ImVec2 mSceneMouse = { 0.0f, 0.0f };


	private:
		void MainMenuBar();
		void Docking();
		bool FullScreenMode{};
		bool Pad{};

	private:

		Window* mWindow{ nullptr };
		ImGuiDockNodeFlags mDockSpaceFlags{};
		static std::unique_ptr<LevelEditor> mInstance;


		//profiler
		bool is_Profile = false;
		float mSceneRuntime = 0.0f;
		float mUpdateTimer = 0.0f;
		std::array<float, 5> mSystemRunTime = { 0.0f,0.0f,0.0f,0.0f };



		//Content Browser
		std::filesystem::path mAssetsPath = "Assets";
		std::filesystem::path mCurrentDirectory = mAssetsPath;

		//Scene/guizmo
		int mGizmoType = -1;

		//For loading of scene usage
		std::vector<std::string> mScenefile; //json file
		// To keep track of the files
		int mSelectedFile = -1;
		std::vector<std::filesystem::directory_entry> mFileList;

		//load audio from file
		std::vector<std::string> mAudioFile;
		std::vector<std::filesystem::directory_entry> mAudioFileList;
		//for displaying of audio channels

		//load texture from file
		std::vector<std::string> mTextureFile;
		std::vector<std::filesystem::directory_entry> mTextureFileList;

		//load script list from file
		std::vector<std::string> mScriptList;
	};
}