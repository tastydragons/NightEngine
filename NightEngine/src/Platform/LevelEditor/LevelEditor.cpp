#include "hzpch.h"


#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "ImGuizmo.h"

#include "LevelEditor.h"
#include "NightEngine/Timer/Timer.h"
#include "NightEngine/Timer/FPS.h"
#include "NightEngine/Renderer/Renderer.h"
#include "NightEngine/Renderer/Graphics.h"
#include "Picker.h"

#include "NightEngine/Renderer/FrameBuffer.h"
#include "NightEngine/Core/Log.h"
#include "NightEngine/Audio/AudioEngine.h"

#include "NightEngine/ECS/Components/Components.h"
#include "NightEngine/ECS/SceneManager.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "NightEngine/Scripts/CollisionResponse.h"
#include "NightEngine/Scripts/ButtonResponse.h"
#include "NightEngine/Scripts/ScenarioScript.h"
#include "NightEngine/Scripts/AudioManager.h"
#include "NightEngine/Scripts/HUDController.h"
#include "NightEngine/Scripts/PlayerControl.h"
#include "NightEngine/Scripts/GateController.h"
#include "NightEngine/Scripts/BackgroundAudio.h"
#include "NightEngine/Scripts/EnemyScript.h"
#include "NightEngine/Scripts/SliderScript.h"
#include "NightEngine/Scripts/PauseMenu.h"
#include "NightEngine/Scripts/HazardScript.h"
#include "NightEngine/Scripts/CutScene.h"
#include "NightEngine/Scripts/DialogueManager.h"
#include "NightEngine/Scripts/BossScript.h"

namespace Night
{
    std::unique_ptr<LevelEditor> LevelEditor::mInstance{ nullptr };

    std::unique_ptr<LevelEditor>& LevelEditor::GetInstance()
    {
        if (mInstance == nullptr)
        {
            mInstance = std::make_unique<LevelEditor>();
        }
        return mInstance;
    }

    bool color_picker = false;
    bool drop_menu = false;
    bool logger = false;
    bool HaltUpdates = true;

    static int current_sound = 0;
    /*!*************************************************************************
    Init loop for levelEditor sets context for ImGui
    ****************************************************************************/
    void LevelEditor::Init(Window* window)
    {
        mWindow = window;

        IMGUI_CHECKVERSION();

        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();

        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

        ImGui_ImplGlfw_InitForOpenGL(window->GetWindow(), true);

        ImGui_ImplOpenGL3_Init("#version 450");

        LoadAudioFromFile();

        LoadSceneFromFile();

        LoadScriptsFromFile();

    }
    /*!*************************************************************************
    Update loop for level editor, poll events and set new frames
    ****************************************************************************/
    void LevelEditor::Update()
    {
#if DEBUG
        if (p_Input->KeyPressed(GLFW_KEY_F1))
        {
            is_ShowWindow = !is_ShowWindow;
        }
#endif
        if (is_ShowWindow)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            Docking();
            MainMenuBar();
            LoadSaveScene();
            Profiler();
            ContentBrowser();
            Logger();
            Hierarchy();
            Inspector();
            SceneViewer();
            AudioEditor();
        }

    }

    /*!*************************************************************************
   Render interface onto frame
   ****************************************************************************/

    void LevelEditor::Draw()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            auto* Window = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(Window);
        }

    }

    /*!*************************************************************************
    End instance of the level editor
    ****************************************************************************/
    void LevelEditor::End()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    /*!*************************************************************************
    Load file paths for scene from assets folder
    ****************************************************************************/
    void LevelEditor::LoadSceneFromFile()
    {
        std::string path = "Assets/Scene";
        for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
        {
            //Checks if the given file status or path corresponds to a regular file
            if (!dir_entry.is_regular_file())
            {
                continue;
            }

            mFileList.emplace_back(dir_entry);
            // used to Load Scene
            mScenefile.emplace_back(dir_entry.path().filename().string());
        }
    }

    /*!*************************************************************************
    Load file paths for textures from assets folder
    ****************************************************************************/
    void LevelEditor::LoadTextureFromFile()
    {
        std::string path = "Assets/Textures";
        for (auto const& dir_entry : std::filesystem::directory_iterator{ path })
        {
            //Checks if the given file status or path corresponds to a regular file
            if (!dir_entry.is_regular_file())
            {
                continue;
            }

            mTextureFileList.emplace_back(dir_entry);
            // used to Load Scene
            mTextureFile.emplace_back(dir_entry.path().filename().string());
        }
    }

    /*!*************************************************************************
   Loads



   filepaths for audio files from assets folder
   ****************************************************************************/
    void LevelEditor::LoadAudioFromFile()
    {
        std::string audio_path = "Assets/metadigger";
        for (auto const& dir_entry : std::filesystem::directory_iterator{ audio_path }) //iterate files in metadigger file
        {
            //checks if given file status or path corresponds to a regular file
            if (!dir_entry.is_regular_file())
            {
                continue;
            }

            mAudioFileList.emplace_back(dir_entry);
            // used to load audio
            mAudioFile.emplace_back(dir_entry.path().filename().string());

        }
    }

    /*!*************************************************************************
    Script filepath to get list of scripts
    ****************************************************************************/
    void LevelEditor::LoadScriptsFromFile()
    {
#if DEBUG
        std::string scriptPath = "../ExoEngine/src/ExoEngine/Scripts";
#else
        std::string scriptPath = "Assets/Scripts"; //for release mode
#endif
        for (auto const& dir_entry : std::filesystem::directory_iterator{ scriptPath })
        {
            if (!dir_entry.is_regular_file())
            {
                continue;
            }
            if (*(dir_entry.path().filename().string().end() - 1) == 'h')
            {
                std::string in = dir_entry.path().filename().string();
                in.pop_back();
                in.pop_back();
                mScriptList.emplace_back(in);
            }
        }
    }

    /*!*************************************************************************
    Menu bar located in the top left side of the window is used to toggle between
    opening and closing the editor
    ****************************************************************************/
    void LevelEditor::MainMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Editor Control"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    is_ShowWindow = true;
                }

                ImGui::Separator();

                if (ImGui::MenuItem("Close"))
                {
                    is_ShowWindow = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    /*!*************************************************************************
   Load and save scenes by serialising and deserializing data from JSON files
   ****************************************************************************/
    void LevelEditor::LoadSaveScene()
    {
        std::vector<const char*> filenames;
        const size_t arrSize = 100;
        const char* fileList[arrSize];
        std::string path = "Assets/Scene/";
        for (auto& item : mScenefile)
        {
            filenames.push_back(item.c_str());
        }

        std::copy(filenames.begin(), filenames.end(), fileList);

        for (size_t i = filenames.size(); i < arrSize; i++)
        {
            fileList[i] = "EMPTY";
        }

        ImGui::Begin("Save / Load");
        ImGui::Combo("##LoadFile", &mSelectedFile, fileList, static_cast<int>(filenames.size()), static_cast<int>(filenames.size()));
        if (mSelectedFile < 0)
        {
            ImGui::End();
            return;
        }
        if (ImGui::Button("LOAD"))
        {
            p_Scene->DeserializeFromFile(path + mFileList[mSelectedFile].path().filename().string());
        }
        ImGui::SameLine();
        ImGui::Button("SAVE");
        if (ImGui::IsItemClicked())
        {
            p_Scene->SerializeToFile(path + mFileList[mSelectedFile].path().filename().string());
        }


        ImGui::End();
    }

    /*!*************************************************************************
    Scene viewer
    ****************************************************************************/
    void LevelEditor::SceneViewer()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Scene");

        auto vpMinRegion = ImGui::GetWindowContentRegionMin();
        auto vpMaxRegion = ImGui::GetWindowContentRegionMax();
        auto vpOffset = ImGui::GetWindowPos();

        mViewportBounds[0] = { vpMinRegion.x + vpOffset.x, vpMinRegion.y + vpOffset.y };
        mViewportBounds[1] = { vpMaxRegion.x + vpOffset.x, vpMaxRegion.y + vpOffset.y };

        //for scrolling purpose
        mViewportFocused = ImGui::IsWindowFocused();
        mViewportSize = { ImGui::GetContentRegionAvail() };

        uint64_t textureID = p_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((void*)(intptr_t)textureID, mViewportSize,
            ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


        //for mouse position
        mGameMousePosition = ImGui::GetMousePos();

        mGameMousePosition.x -= mViewportBounds[0].x;
        mGameMousePosition.y -= mViewportBounds[0].y;

        glm::vec2 vpSize{ 0.0f ,0.0f };
        vpSize.x = mViewportBounds[1].x - mViewportBounds[0].x;
        vpSize.y = mViewportBounds[1].y - mViewportBounds[0].y;
        mGameMousePosition.y = vpSize.y - mGameMousePosition.y;
        mGameMousePosition.x = ((mGameMousePosition.x / mViewportSize.x) * 2.0f) - 1.0f;
        mGameMousePosition.y = ((mGameMousePosition.y / mViewportSize.y) * 2.0f) - 1.0f;
        static bool dragging = false;
        static double c_x = 0.0f, c_y = 0.0f;
        if (p_Input->MousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        {
            if (mSceneMouse.x >= mViewportBounds[0].x &&
                mSceneMouse.x <= mViewportBounds[1].x &&
                mSceneMouse.y >= mViewportBounds[0].y &&
                mSceneMouse.y <= mViewportBounds[1].y)
            {
                dragging = true;
            }
        }
        if (dragging && p_Input->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
        {
            double new_x, new_y;
            glfwGetCursorPos(mWindow->GetWindow(), &new_x, &new_y);

            Night::Graphic::scene_camera.SetPosition({ Night::Graphic::scene_camera.GetPosition().x - (static_cast<float>(new_x - c_x) * 0.005f) ,
                                                    Night::Graphic::scene_camera.GetPosition().y + (static_cast<float>(new_y - c_y) * 0.005f),
                0.0f });

        }
        else if (dragging && p_Input->MouseIsReleased(GLFW_MOUSE_BUTTON_RIGHT))
        {
            dragging = false;
        }
        glfwGetCursorPos(mWindow->GetWindow(), &c_x, &c_y);
        //gizmos
        if (p_Input->isKeyPressed(GLFW_KEY_1) && !ImGuizmo::IsUsing() && mViewportFocused)
        {
            mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
        }
        else if (p_Input->isKeyPressed(GLFW_KEY_2) && !ImGuizmo::IsUsing() && mViewportFocused)
        {
            mGizmoType = ImGuizmo::OPERATION::ROTATE;
        }
        else if (p_Input->isKeyPressed(GLFW_KEY_3) && !ImGuizmo::IsUsing() && mViewportFocused)
        {
            mGizmoType = ImGuizmo::OPERATION::SCALE;
        }
        ImGuizmo::BeginFrame();
        if (selectedEntity >= 0 && selectedEntity < MAX_ENTITIES && p_ecs.HaveComponent<Transform>(selectedEntity))//have selected entity
        {
            ImGuizmo::SetOrthographic(true);


            ImGuizmo::SetRect(
                mViewportBounds[0].x,
                mViewportBounds[0].y,
                mViewportBounds[1].x - mViewportBounds[0].x,
                mViewportBounds[1].y - mViewportBounds[0].y
            );

            glm::mat4 cameraProj = Night::Graphic::mcamera->GetProjectionMatrix();
            glm::mat4 cameraView = Night::Graphic::mcamera->GetViewMatrix();
            glm::mat4 transform{ 1.0f }; // identity matrix

            auto& trans = p_ecs.GetComponent<Transform>(selectedEntity);

            transform = glm::translate(glm::mat4{ 1.0f }, glm::vec3(trans.GetPos().x, trans.GetPos().y, 0.0f))
                * glm::rotate(glm::mat4(1.0f), glm::radians(trans.GetRot()), glm::vec3(0.0f, 0.0f, 1.0f))
                * glm::scale(glm::mat4(1.f), { trans.GetScale().x,trans.GetScale().y ,1.0f });

            ImGuizmo::SetDrawlist();
            // Draw ImGuizmo (renders every frame)
            ImGuizmo::Manipulate(glm::value_ptr(cameraView)
                , glm::value_ptr(cameraProj), (ImGuizmo::OPERATION)mGizmoType,
                ImGuizmo::WORLD, glm::value_ptr(transform));

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 decompTrans, decompRot, decompScale;

                ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(transform),
                    glm::value_ptr(decompTrans),
                    glm::value_ptr(decompRot),
                    glm::value_ptr(decompScale));
                // change size 
                trans.SetPos(decompTrans[0], decompTrans[1]);
                trans.SetRot(decompRot[2]);
                trans.SetScale(decompScale[0], decompScale[1]);

            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
        std::multimap<float, Transform*> sortedMultimap;
        for (Entity entity = 0; entity < p_ecs.GetTotalEntities(); entity++)
        {
            if (p_ecs.HaveComponent<Transform>(entity))
                sortedMultimap.insert({ p_ecs.GetComponent<Transform>(entity).GetRot(),
                            &p_ecs.GetComponent<Transform>(entity) });
        }
        if (p_ecs.GetTotalEntities() != 0 && p_Input->MousePressed(GLFW_MOUSE_BUTTON_LEFT) && !ImGuizmo::IsOver())
        {
            if (selectedEntity >= 0 && selectedEntity < MAX_ENTITIES)//no entity selected will remain to the previous selected entity
            {
                if (p_ecs.HaveComponent<Transform>(selectedEntity))
                    selectedEntity = (Entity)Picker::Pick(Night::Graphic::mcamera, sortedMultimap);
            }
            else
                selectedEntity = 0;

        }
    }

    /*!*************************************************************************
   View various assets from the assets folders from the level editor
   ****************************************************************************/
    void LevelEditor::ContentBrowser()
    {
        if (is_ShowWindow)
        {
            ImGui::Begin("Content Browser");
            if (mCurrentDirectory != std::filesystem::path(mAssetsPath))
            {
                if (ImGui::ImageButton((void*)(intptr_t)ResourceManager::GetIcon("BackIcon")->GetRendererID(),
                    { 25.f,25.f }, { 0, 1 }, { 1, 0 }))
                {
                    mCurrentDirectory = mCurrentDirectory.parent_path();
                }
            }

            for (auto& Directory : std::filesystem::directory_iterator(mCurrentDirectory))
            {
                const auto& directorypath = Directory.path(); //the path for folders in Assets(assets/fonts)

                std::string filename = directorypath.filename().string(); //just the name of the folders(fonts, icons, metadigger etc)
                ImGui::PushID(filename.c_str());
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

                if (Directory.is_directory())
                {
                    ImGui::ImageButton((void*)(intptr_t)ResourceManager::GetIcon("FolderIcon")->GetRendererID(),
                        { 128.f,128.f }, { 0, 1 }, { 1, 0 });
                }

                //Press into the folder
                ImGui::PopStyleColor();
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    if (Directory.is_directory())
                        mCurrentDirectory /= directorypath.filename();
                }

                ImGui::TextWrapped(filename.c_str()); ImGui::NextColumn(); //indicate the folder name
                ImGui::PopID();
            }

            ImGui::End();

            ImGui::Begin(mCurrentDirectory.filename().string().c_str(), (bool*)0, ImGuiWindowFlags_HorizontalScrollbar);
            if (mCurrentDirectory.filename() == "Textures")
            {
                for (auto& [name, texObj] : ResourceManager::textures)
                {
                    ImGui::Image((void*)(intptr_t)texObj->GetRendererID(),
                        ImVec2(128 * static_cast<float>(texObj->GetWidth()) / static_cast<float>(texObj->GetHeight()), 128),
                        ImVec2(0.0f, 1.0f),
                        ImVec2(1.0f, 0.0f));

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        std::string str = name + " " + "(" + std::to_string(texObj->GetWidth()) + "x" +
                            std::to_string(texObj->GetHeight()) + ")";
                        ImGui::Text(str.c_str());
                        ImGui::EndTooltip();
                    }

                    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                    {
                        ImGui::SetDragDropPayload("Textures", &name, sizeof(name));
                        ImGui::Image((void*)(intptr_t)texObj->GetRendererID(),
                            ImVec2(128 * static_cast<float>(texObj->GetWidth()) / static_cast<float>(texObj->GetHeight()), 128),
                            ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
                        ImGui::EndDragDropSource();
                    }
                }
            }
            else if (mCurrentDirectory.filename() == "Icons")
            {
                for (auto& [name, texObj] : ResourceManager::Icons)
                {
                    ImGui::SameLine();
                    ImGui::Image((void*)(intptr_t)texObj->GetRendererID(),
                        ImVec2(128 * static_cast<float>(texObj->GetWidth()) / static_cast<float>(texObj->GetHeight()), 128),
                        ImVec2(0.0f, 1.0f),
                        ImVec2(1.0f, 0.0f));

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        std::string str = name + " " + "(" + std::to_string(texObj->GetWidth()) + "x" +
                            std::to_string(texObj->GetHeight()) + ")";
                        ImGui::Text(str.c_str());
                        ImGui::EndTooltip();
                    }
                }

            }
            ImGui::End();
        }

    }
    /*!*************************************************************************
   Logger, can toggle between types of messages you want to view
   ****************************************************************************/
    void LevelEditor::Logger()
    {
        if (is_ShowWindow)
        {

            if (ImGui::Begin("Logger"))
            {
                static bool info = true;
                ImGui::Checkbox("Info       ", &info);

                ImGui::SameLine();

                static bool warning = true;
                ImGui::Checkbox("warning    ", &warning);

                ImGui::SameLine();

                static bool error = true;
                ImGui::Checkbox("error", &error);

                static bool system = true;
                ImGui::Checkbox("system     ", &system);

                ImGui::SameLine();

                static bool log = true;
                ImGui::Checkbox("log        ", &log);

                ImGui::SameLine();

                static bool fatal = true;
                ImGui::Checkbox("fatal", &fatal);

                if (info)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                    ImGui::Text(Log::GetImguiLog().c_str());
                    ImGui::PopStyleColor();



                }

                if (warning)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
                    ImGui::Text("This is an Warning Message");
                    ImGui::PopStyleColor();
                }

                if (error)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    ImGui::Text("This is an Error Message");
                    ImGui::PopStyleColor();
                }

                if (system)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(52, 67, 235, 255));
                    ImGui::Text("This is an System Message");
                    ImGui::PopStyleColor();

                    if (p_Audio->GetPlayingChannels() > 0)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                        ImGui::Text("Audio Channel %d is played", p_Audio->GetPlayingChannels());
                        ImGui::PopStyleColor();
                    }
                }

                if (log)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(52, 222, 235, 255));
                    ImGui::Text("This is an Log Message");
                    ImGui::PopStyleColor();
                }

                if (fatal)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    ImGui::Text("This is an Fatal Message");
                    ImGui::PopStyleColor();

                }
            }
            ImGui::End();
        }
    }

    /*!*************************************************************************
   Docking allows us to dock the ImGui windows to the edges of the scene
   ****************************************************************************/
    void LevelEditor::Docking()
    {
        //Set docking flags
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
            ImGuiWindowFlags_NoBackground;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", 0, window_flags);

        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::End();

    }


    /*!*************************************************************************
    Profiler displays information about the users system such as

    Renderer and version of renderer (GPU)

    FPS

    Rendered info e.g. Quads, vertices, indices drawn etc.
    ****************************************************************************/
    void LevelEditor::Profiler()
    {
        if (is_ShowWindow)
        {
            ImGui::Begin("Profiler");
            //Opengl information
            ImGui::Text("Opengl Information ");
            ImGui::Text("Vendor: %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
            ImGui::Text("Version: %s", glGetString(GL_VERSION));
            ImGui::Text("\n");

            // how much time per frame and Fps
            ImGui::Text("Application average % .3f ms / frame(% .3f FPS)", 1000.0f / FPS::GetInstance().GetFps(), FPS::GetInstance().GetFps());

            //Renderering Information
            ImGui::Text("\n");
            auto Infos = Renderer::GetInfo();
            ImGui::Text("Renderer Information");
            ImGui::Text("Draw Calls: %d", Infos.n_DrawCalls);
            ImGui::Text("Quads: %d", Infos.n_Quad);
            ImGui::Text("Vertices: %d", Infos.TotalVertexUsed());
            ImGui::Text("Indices: %d", Infos.TotalIndexUsed());

            ImGui::Checkbox("Show physics colliders", &mDebugDraw);

            mSystemRunTime[0] = Timer::GetInstance().GetDT(Systems::COLLISION) / Timer::GetInstance().GetGlobalDT();
            mSystemRunTime[1] = Timer::GetInstance().GetDT(Systems::GRAPHIC) / Timer::GetInstance().GetGlobalDT();
            mSystemRunTime[2] = Timer::GetInstance().GetDT(Systems::PHYSICS) / Timer::GetInstance().GetGlobalDT();
            mSystemRunTime[3] = Timer::GetInstance().GetDT(Systems::LOGIC) / Timer::GetInstance().GetGlobalDT();
            mSystemRunTime[4] = Timer::GetInstance().GetDT(Systems::API) / Timer::GetInstance().GetGlobalDT();

            ImGui::Text("COLLISION");
            ImGui::SameLine(80);
            ImGui::ProgressBar(mSystemRunTime[0], ImVec2(-1.0f, 0.0f));
            ImGui::Text("GRAPHIC");
            ImGui::SameLine(80);
            ImGui::ProgressBar(mSystemRunTime[1], ImVec2(-1.0f, 0.0f));
            ImGui::Text("PHYSICS");
            ImGui::SameLine(80);
            ImGui::ProgressBar(mSystemRunTime[2], ImVec2(-1.0f, 0.0f));
            ImGui::Text("LOGIC");
            ImGui::SameLine(80);
            ImGui::ProgressBar(mSystemRunTime[3], ImVec2(-1.0f, 0.0f));
            ImGui::Text("API");
            ImGui::SameLine(80);
            ImGui::ProgressBar(mSystemRunTime[4], ImVec2(-1.0f, 0.0f));

            ImGui::End();


        }
    }

    /*!*************************************************************************
  // Create, destroy and clone entities in the hirarchy window
  ****************************************************************************/
    void LevelEditor::Hierarchy()
    {
        if (is_ShowWindow)
        {
            ImGui::Begin("Hierarchy");
            if (ImGui::Button("Create Entity") && p_ecs.GetTotalEntities() != MAX_ENTITIES)
            {
                p_ecs.AddComponent<NameTag>(p_ecs.CreateEntity(), C_NameTagComponent);
            }

            if (p_ecs.GetTotalEntities() > 0)
            {
                ImGui::SameLine();
                if (ImGui::Button("Destroy Entity"))
                {
                    if (selectedEntity != MAX_ENTITIES)
                    {
                        p_ecs.DestroyEntity(selectedEntity);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Clone Entity") && p_ecs.GetTotalEntities() != 0)// there is entity alive
                {
                    if (selectedEntity != MAX_ENTITIES)
                    {
                        Entity CloneEntity = p_ecs.CloneEntity(selectedEntity);
                        selectedEntity = CloneEntity; // when the entity is destroy there is no current selected entity
                    }
                }

                auto aliveTotal = p_ecs.GetTotalEntities();
                Entity aliveCount = 0;
                Entity iterEntity = 0;
                while (aliveCount < aliveTotal)
                {
                    if (p_ecs.HaveComponent<NameTag>(iterEntity))
                    {
                        const auto& tag = p_ecs.GetComponent<NameTag>(iterEntity).GetNameTag();

                        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnArrow;
                        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)iterEntity, flags, tag.c_str());

                        if (ImGui::IsItemClicked())
                            selectedEntity = iterEntity;

                        if (opened)
                            ImGui::TreePop();

                        aliveCount++;
                    }

                    iterEntity++;
                }
            }

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                selectedEntity = {};
            ImGui::End();
        }

    }

    /*!*************************************************************************
   Inspector allows us to manipulate the entity properties, modifying scale,
   rotation and position of the object.
   ****************************************************************************/
    void LevelEditor::Inspector()
    {
        if (is_ShowWindow)
        {
            ImGui::Begin("Inspector");
            if (selectedEntity < MAX_ENTITIES && selectedEntity >= 0 && p_ecs.GetTotalEntities() != 0)// if the selectedEntityExist
            {
                //create component for the selected entity 
                if (ImGui::Button("Add Component"))
                    ImGui::OpenPopup("Add Component");

                if (ImGui::BeginPopup("Add Component"))
                {

                    if (ImGui::MenuItem("Transform"))
                    {
                        if (!p_ecs.HaveComponent<Transform>(selectedEntity))
                            p_ecs.AddComponent<Transform>(selectedEntity, C_TransformComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Sprite"))
                    {
                        if (!p_ecs.HaveComponent<Sprite>(selectedEntity))
                            p_ecs.AddComponent<Sprite>(selectedEntity, C_SpriteComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Collider"))
                    {
                        if (!p_ecs.HaveComponent<Collider>(selectedEntity))
                            p_ecs.AddComponent<Collider>(selectedEntity, C_ColliderComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("RigidBody"))
                    {
                        if (!p_ecs.HaveComponent<RigidBody>(selectedEntity))
                            p_ecs.AddComponent<RigidBody>(selectedEntity, C_RigidBodyComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("PlayerAttributes"))
                    {
                        if (!p_ecs.HaveComponent<PlayerAttributes>(selectedEntity))
                            p_ecs.AddComponent<PlayerAttributes>(selectedEntity, C_PlayerAttributesComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Logic"))
                    {
                        if (!p_ecs.HaveComponent<Logic>(selectedEntity))
                            p_ecs.AddComponent<Logic>(selectedEntity, C_LogicComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Audio"))
                    {
                        if (!p_ecs.HaveComponent<Audio>(selectedEntity))
                            p_ecs.AddComponent<Audio>(selectedEntity, C_AudioComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Tag"))
                    {
                        if (!p_ecs.HaveComponent<Tag>(selectedEntity))
                            p_ecs.AddComponent<Tag>(selectedEntity, C_TagComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("HUD"))
                    {
                        if (!p_ecs.HaveComponent<HUDComponent>(selectedEntity))
                            p_ecs.AddComponent<HUDComponent>(selectedEntity, C_HUDComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("EnemyAttributes"))
                    {
                        if (!p_ecs.HaveComponent<EnemyAttributes>(selectedEntity))
                            p_ecs.AddComponent<EnemyAttributes>(selectedEntity, C_EnemyAttributesComponent);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Attributes"))
                    {
                        if (!p_ecs.HaveComponent<Attributes>(selectedEntity))
                            p_ecs.AddComponent<Attributes>(selectedEntity, C_AttributesComponent);
                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
                //Check and change the name of the Entity
                if (p_ecs.HaveComponent<NameTag>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Name", ImGuiTreeNodeFlags_None))
                    {
                        auto& name = p_ecs.GetComponent<NameTag>(selectedEntity).GetNameTag();
                        char buffer[256];
                        memset(buffer, 0, sizeof(buffer));
                        strcpy_s(buffer, sizeof(buffer), name.c_str());
                        if (ImGui::InputText("name", buffer, sizeof(buffer)))
                        {
                            name = std::string(buffer);
                        }
                    }
                }
                //If Entity have Transform Component
                if (p_ecs.HaveComponent<Transform>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
                    {
                        //position
                        auto& Position = p_ecs.GetComponent<Transform>(selectedEntity).GetPos();
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("Position"); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();                    //set a "x" to indicate x-axis
                        ImGui::DragFloat("##PositionX", (float*)&Position.x, 0.005f); ImGui::SameLine(); //char name , pass float pointer to position vec2D which hold x and y, the scaling value in imgui
                        ImGui::PushID(1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##PositionY", (float*)&Position.y, 0.005f);
                        ImGui::PopID();

                        //scale
                        auto& Scale = p_ecs.GetComponent<Transform>(selectedEntity).GetScale();
                        ImGui::Text("Scale   "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();                    //set a "x" to indicate x-axis
                        ImGui::DragFloat("##Scale", (float*)&Scale.x, 0.005f); ImGui::SameLine(); //char name , pass float pointer to position vec2D which hold x and y, the scaling value in imgui
                        ImGui::PushID(2);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##Scale", (float*)&Scale.y, 0.005f);
                        ImGui::PopID();

                        //rotation
                        auto& rotation = p_ecs.GetComponent<Transform>(selectedEntity).GetRot();
                        ImGui::Text("Rotation Z"); ImGui::SameLine();
                        ImGui::DragFloat("##", (float*)&rotation, 1.0f);
                    }
                }
                //Sprite Component
                if (p_ecs.HaveComponent<Sprite>(selectedEntity))
                {

                    if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_None))
                    {
                        auto& sprite = p_ecs.GetComponent<Sprite>(selectedEntity);
                        ImGui::Checkbox("SpriteSheet", &sprite.is_SpriteSheet); ImGui::SameLine();
                        ImGui::Checkbox("Animation", &sprite.is_Animated);
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("Coordinates: "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::DragInt("##X", (int*)&sprite.GetIndex().x, 1); ImGui::SameLine();
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragInt("##Y", (int*)&sprite.GetIndex().y, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4());
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4());
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4());
                        ImGui::Text("Image : "); ImGui::SameLine();
                        ImGui::PopStyleColor(3);
                        auto& texturePath = p_ecs.GetComponent<Sprite>(selectedEntity).GetTexture();
                        ImGui::SetNextItemWidth(140.0f);

                        if (ImGui::BeginCombo("##sprite", texturePath.c_str()))
                        {
                            for (auto& [str, tex] : ResourceManager::textures)
                            {
                                if (ImGui::Selectable(str.c_str()))
                                {
                                    texturePath = str;
                                    NIGHT_CORE_INFO("Loaded {0} Sprite", texturePath.c_str());
                                }
                            }
                            ImGui::EndCombo();
                        }

                        if (ImGui::BeginDragDropTarget())
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Textures"))
                            {
                                texturePath = *(const std::string*)payload->Data;
                            }
                            ImGui::EndDragDropTarget();
                        }
                        if (sprite.is_SpriteSheet)
                        {
                            sprite.GetMaxIndex() = (int)GETTEXTURE(sprite.GetTexture())->GetWidth() / 512;
                            ImGui::Text("MaxIndex : %d ", sprite.GetMaxIndex());
                            sprite.GetDisplayTime().resize(sprite.GetMaxIndex());//resize the number of frames in a sprite  
                        }
                        if (sprite.GetMaxIndex() > 0 && sprite.is_Animated)
                        {
                            ImGui::Text("SelectedIndex: "); ImGui::SameLine();
                            ImGui::DragInt("##SelectedIndex", (int*)&sprite.GetIndex().x, 1, 0, sprite.GetMaxIndex() - 1);

                            ImGui::Text("SetDisplayTime: "); ImGui::SameLine();
                            ImGui::DragFloat("##SetDisplayTime", (float*)&sprite.GetDisplayTime()[sprite.GetIndex().x], 0.005f, 0.0f);

                        }

                        if (sprite.is_SpriteSheet)
                        {
                            ImGui::Text("UVCoordinates: "); ImGui::SameLine();
                            ImGui::Text("U"); ImGui::SameLine();
                            ImGui::DragFloat("##U", (float*)&sprite.GetUVCoor().x, 0.5f); ImGui::SameLine();
                            ImGui::PushID(2);
                            ImGui::Text("V"); ImGui::SameLine();
                            ImGui::DragFloat("##V", (float*)&sprite.GetUVCoor().y, 0.5f);
                            ImGui::PopID();
                        }
                        ImGui::Text("Layering Order: "); ImGui::SameLine();
                        ImGui::DragInt("##layering", (int*)&sprite.LayerOrder, 1, 0, 8);
                    }
                }
                //Collider Component
                if (p_ecs.HaveComponent<Collider>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Collider", ImGuiTreeNodeFlags_None))
                    {
                        //selection for collider type
                        auto& colliderComp = p_ecs.GetComponent<Collider>(selectedEntity);
                        auto& colliderType = colliderComp[0].mCol;
                        int colliderIndex = static_cast<int>(colliderType);
                        const char* colliderNames = "none\0circle\0bubble\0line\0rect\0box\0bossball\0button";
                        ImGui::Text("Collider 1 Type"); ImGui::SameLine();
                        ImGui::Combo("##test", &colliderIndex, colliderNames);
                        colliderType = static_cast<Collider::ColliderType>(colliderIndex);

                        //positioning the offset
                        auto& colliderOffset = colliderComp[0].mOffset;
                        ImGui::PushItemWidth(100.0f);

                        ImGui::Text("OffSet   "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();

                        ImGui::DragFloat("##colliderOffsetX", (float*)&colliderOffset.x, 0.05f); ImGui::SameLine();
                        ImGui::PushID(1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##colliderOffsetY", (float*)&colliderOffset.y, 0.05f);
                        ImGui::PopID();

                        //size of the collider
                        if (colliderComp[0].mCol == Collider::ColliderType::circle || colliderComp[0].mCol == Collider::ColliderType::bubble ||
                            colliderComp[0].mCol == Collider::ColliderType::bossball)
                        {
                            auto& colliderSize = colliderComp[0].mRadius;
                            ImGui::Text("Radius   "); ImGui::SameLine();
                            ImGui::DragFloat("##Radius", (float*)&colliderSize, 0.05f);
                        }
                        else if (colliderComp[0].mCol == Collider::ColliderType::rect || colliderComp[0].mCol == Collider::ColliderType::box)
                        {
                            ImGui::Text("Minimum "); ImGui::SameLine();
                            ImGui::Text("X"); ImGui::SameLine();
                            ImGui::DragFloat("##MinimumX", (float*)&colliderComp[0].mMin.x, 0.05f); ImGui::SameLine();
                            ImGui::Text("Y"); ImGui::SameLine();
                            ImGui::DragFloat("##MinimumY", (float*)&colliderComp[0].mMin.y, 0.05f);

                            ImGui::Text("Maximum "); ImGui::SameLine();
                            ImGui::Text("X"); ImGui::SameLine();
                            ImGui::DragFloat("##MaximumX", (float*)&colliderComp[0].mMax.x, 0.05f); ImGui::SameLine();
                            ImGui::Text("Y"); ImGui::SameLine();
                            ImGui::DragFloat("##MaximumY", (float*)&colliderComp[0].mMax.y, 0.05f);
                        }

                        //selection for collider 2 type
                        auto& colliderType1 = colliderComp[1].mCol;
                        int colliderIndex1 = static_cast<int>(colliderType1);
                        const char* colliderNames1 = "none\0circle\0bubble\0line\0rect\0box\0button";
                        ImGui::Text("Collider 2 Type"); ImGui::SameLine();
                        ImGui::Combo("###test", &colliderIndex1, colliderNames1);
                        ImGui::PushItemWidth(100.0f);
                        colliderType1 = static_cast<Collider::ColliderType>(colliderIndex1);

                        //positioning the offset
                        auto& colliderOffset1 = colliderComp[1].mOffset;

                        ImGui::Text("OffSet   "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();

                        ImGui::DragFloat("###colliderOffsetX", (float*)&colliderOffset1.x, 0.05f); ImGui::SameLine();
                        ImGui::PushID(1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("###colliderOffsetY", (float*)&colliderOffset1.y, 0.05f);
                        ImGui::PopID();

                        //size of the collider
                        if (colliderComp[1].mCol == Collider::ColliderType::circle || colliderComp[1].mCol == Collider::ColliderType::bubble ||
                            colliderComp[1].mCol == Collider::ColliderType::bossball)
                        {
                            auto& colliderSize1 = colliderComp[1].mRadius;
                            ImGui::Text("Radius   "); ImGui::SameLine();
                            ImGui::DragFloat("###Radius", (float*)&colliderSize1, 0.05f);
                        }
                        else if (colliderComp[1].mCol == Collider::ColliderType::rect || colliderComp[1].mCol == Collider::ColliderType::box)
                        {
                            ImGui::Text("Minimum "); ImGui::SameLine();
                            ImGui::Text("X"); ImGui::SameLine();
                            ImGui::DragFloat("###MinimumX", (float*)&colliderComp[1].mMin.x, 0.05f); ImGui::SameLine();
                            ImGui::Text("Y"); ImGui::SameLine();
                            ImGui::DragFloat("###MinimumY", (float*)&colliderComp[1].mMin.y, 0.05f);

                            ImGui::Text("Maximum "); ImGui::SameLine();
                            ImGui::Text("X"); ImGui::SameLine();
                            ImGui::DragFloat("###MaximumX", (float*)&colliderComp[1].mMax.x, 0.05f); ImGui::SameLine();
                            ImGui::Text("Y"); ImGui::SameLine();
                            ImGui::DragFloat("###MaximumY", (float*)&colliderComp[1].mMax.y, 0.05f);
                        }
                    }
                }
                //Player Attributes
                if (p_ecs.HaveComponent<PlayerAttributes>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("PlayerAttributes", ImGuiTreeNodeFlags_None))
                    {
                        auto& attrib = p_ecs.GetComponent<PlayerAttributes>(selectedEntity);

                        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
                        int healthSlider = attrib.mHealth;
                        int maxHealthSlider = attrib.mMaxHealth;
                        int damageSlider = attrib.mDamage;

                        ImGui::SliderInt("Health (0 -> 150)", &healthSlider, 0, 150, "%d", flags);
                        ImGui::SliderInt("Max Health (0 -> 200)", &maxHealthSlider, 0, 200, "%d", flags);
                        ImGui::SliderInt("Damage (0 -> 50)", &damageSlider, 0, 50, "%d", flags);

                        attrib.mHealth = healthSlider;
                        attrib.mMaxHealth = maxHealthSlider;
                        attrib.mDamage = damageSlider;
                    }
                }
                //Attributes
                if (p_ecs.HaveComponent<Attributes>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Attributes", ImGuiTreeNodeFlags_None))
                    {
                        auto& attrib = p_ecs.GetComponent<Attributes>(selectedEntity);

                        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
                        float FiringCoolDown = attrib.mFiringCoolDown;
                        float FireDurationTimer = attrib.mFireDurationTimer;
                        float ChargeCoolDown = attrib.mChargeCoolDown;
                        int HazardDmg = attrib.mHazardDmg;
                        const char* HazardList = "None\0Ground\0Laser";
                        auto HazardType = attrib.mHazardType;
                        int HazardIndex = static_cast<int>(HazardType);
                        ImGui::Combo("HazardType", &HazardIndex, HazardList);
                        ImGui::PushItemWidth(100.0f);
                        attrib.mHazardType = static_cast<Attributes::HazardTypes>(HazardIndex);

                        ImGui::SliderFloat("Firing Cooldown (0 -> 10)", &FiringCoolDown, 0, 10, "%f", flags);
                        ImGui::SliderFloat("Firing Duration (0 -> 10)", &FireDurationTimer, 0, 10, "%f", flags);
                        ImGui::SliderFloat("Charge Cooldown (0 -> 10)", &ChargeCoolDown, 0, 10, "%f", flags);
                        ImGui::SliderInt("Damage (0 -> 50)", &HazardDmg, 0, 50, "%f", flags);


                        attrib.mFiringCoolDown = FiringCoolDown;
                        attrib.mFireDurationTimer = FireDurationTimer;
                        attrib.mChargeCoolDown = ChargeCoolDown;
                        attrib.mHazardDmg = HazardDmg;
                    }
                }
                //Logic
                if (p_ecs.HaveComponent<Logic>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Logic", ImGuiTreeNodeFlags_None))
                    {
                        auto& logic = p_ecs.GetComponent<Logic>(selectedEntity);

                        static ImGuiComboFlags flags = 0;
                        static int current_script = 0;
                        const size_t aSize = 30;
                        const char* logicList[aSize];
                        std::vector<const char*> sList;

                        for (int i = 0; i < mScriptList.size(); i++)
                        {
                            sList.push_back(mScriptList[i].c_str());
                        }
                        std::copy(sList.begin(), sList.end(), logicList);

                        ImGui::Combo("Logic Scripts", &current_script, logicList, static_cast<int>(sList.size()), static_cast<int>(sList.size()));

                        if (ImGui::Button("Add"))
                        {
                            bool can_addScript = true;
                            for (int i = 0; i < logic.GetScriptNames().size(); i++)
                            {
                                if (logic.GetScriptNames()[i] == mScriptList[current_script])
                                {
                                    can_addScript = false;
                                }
                            }
                            if (can_addScript == true)
                            {
                                if (p_ecs.HaveComponent<NameTag>(selectedEntity) && p_ecs.HaveComponent<Transform>(selectedEntity))
                                {
                                    if (mScriptList[current_script] == "PlayerControl" && p_ecs.HaveComponent<RigidBody>(selectedEntity) && p_ecs.HaveComponent<PlayerAttributes>(selectedEntity)
                                        && p_ecs.HaveComponent<Sprite>(selectedEntity) && p_ecs.HaveComponent<Collider>(selectedEntity))
                                    {
                                        logic.InsertScript(new PlayerControl(selectedEntity), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "CollisionResponse" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<RigidBody>(selectedEntity))
                                    {
                                        logic.InsertScript(new CollisionResponse(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "ButtonResponse" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Tag>(selectedEntity)
                                        && p_ecs.HaveComponent<Sprite>(selectedEntity))
                                    {
                                        logic.InsertScript(new ButtonResponse(), selectedEntity);
                                    }

                                    if (mScriptList[current_script] == "PauseMenu" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Tag>(selectedEntity)
                                        && p_ecs.HaveComponent<Sprite>(selectedEntity))
                                    {
                                        logic.InsertScript(new PauseMenu(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "ScenerioScript")
                                    {
                                        logic.InsertScript(new ScenerioScript(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "AudioManager" && p_ecs.HaveComponent<Audio>(selectedEntity))
                                    {
                                        logic.InsertScript(new AudioManager(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "HUDController" && p_ecs.HaveComponent<HUDComponent>(selectedEntity) && p_ecs.HaveComponent<Sprite>(selectedEntity))
                                    {
                                        logic.InsertScript(new HUDController(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "GateController" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Sprite>(selectedEntity))
                                    {
                                        logic.InsertScript(new GateController(selectedEntity), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "EnemyScript" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Sprite>(selectedEntity)
                                        && p_ecs.HaveComponent<EnemyAttributes>(selectedEntity))
                                    {
                                        logic.InsertScript(new EnemyScript(selectedEntity), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "BackgroundAudio" && p_ecs.HaveComponent<Audio>(selectedEntity))
                                    {
                                        logic.InsertScript(new BackgroundAudio(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "SliderScript" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Tag>(selectedEntity)
                                        && p_ecs.HaveComponent<Sprite>(selectedEntity) && p_ecs.HaveComponent<NameTag>(selectedEntity))
                                    {
                                        logic.InsertScript(new SliderScript(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "HazardScript" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Tag>(selectedEntity)
                                        && p_ecs.HaveComponent<Sprite>(selectedEntity) && p_ecs.HaveComponent<Attributes>(selectedEntity))
                                    {
                                        logic.InsertScript(new HazardScript(selectedEntity), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "CutScene" && p_ecs.HaveComponent<Sprite>(selectedEntity) && p_ecs.HaveComponent<NameTag>(selectedEntity))
                                    {
                                        logic.InsertScript(new CutScene(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "DialogueManager" && p_ecs.HaveComponent<Sprite>(selectedEntity))
                                    {
                                        logic.InsertScript(new DialogueManager(), selectedEntity);
                                    }
                                    if (mScriptList[current_script] == "BossScript" && p_ecs.HaveComponent<Collider>(selectedEntity) && p_ecs.HaveComponent<Sprite>(selectedEntity)
                                        && p_ecs.HaveComponent<EnemyAttributes>(selectedEntity))
                                    {
                                        logic.InsertScript(new BossScript(selectedEntity), selectedEntity);
                                    }
                                }
                            }
                        }
                        ImGui::SameLine();
                        if (ImGui::Button("Delete")) {
                            logic.DeleteScript(mScriptList[current_script]);
                        }
                        ImGui::Text("Inserted Scripts:");
                        for (int i = 0; i < logic.GetScriptNames().size(); i++) {
                            //display the names of all scripts in the entity
                            ImGui::Text("% s", logic.GetScriptNames()[i].c_str());
                        }
                    }
                }
                //Rigid Component
                if (p_ecs.HaveComponent<RigidBody>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("RigidBody", ImGuiTreeNodeFlags_None))
                    {
                        //Acceleration
                        auto& velocity = p_ecs.GetComponent<RigidBody>(selectedEntity).GetAccel();
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("Acceleration   "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::DragFloat("##AccelerationX", (float*)&velocity.x, 0.005f); ImGui::SameLine();
                        ImGui::PushID(3);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##AccelerationY", (float*)&velocity.y, 0.005f);
                        ImGui::PopID();

                        //Direction
                        auto& direction = p_ecs.GetComponent<RigidBody>(selectedEntity).GetDir();
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("Direction  "); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::DragFloat("##DirectionX", (float*)&direction.x, 0.005f); ImGui::SameLine();
                        ImGui::PushID(4);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##DirectionY", (float*)&direction.y, 0.005f);
                        ImGui::PopID();

                        //GetFricition
                        auto& friction = p_ecs.GetComponent<RigidBody>(selectedEntity).GetFriction();
                        ImGui::Text("Friction   "); ImGui::SameLine();
                        ImGui::DragFloat("##Friction", (float*)&friction, 1.0f);

                        //GetRestitution
                        auto& Restitution = p_ecs.GetComponent<RigidBody>(selectedEntity).GetRestitution();
                        ImGui::Text("Restitution"); ImGui::SameLine();
                        ImGui::DragFloat("##Restitution", (float*)&Restitution, 1.0f);
                    }
                }
                //Audio Component
                if (p_ecs.HaveComponent<Audio>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Audio", ImGuiTreeNodeFlags_None))
                    {
                        auto& AudioComp = p_ecs.GetComponent<Audio>(selectedEntity);
                        auto& AudioArr = AudioComp.GetArr();
                        //list out all audio files loaded
                        static int currentfile = 0;
                        const size_t arraysize = 30;
                        const char* audioFileList[arraysize];
                        std::vector<const char*> audiofilenames;
                        for (auto& item : mAudioFile) {
                            audiofilenames.push_back(item.c_str());
                        }
                        std::copy(audiofilenames.begin(), audiofilenames.end(), audioFileList);
                        ImGui::Text("Loaded Audio Files:");
                        ImGui::Combo("##Audio Files", &currentfile, audioFileList, static_cast<int>(audiofilenames.size()), static_cast<int>(audiofilenames.size()));
                        //list out audio file paths in component
                        static ImGuiComboFlags flags = 0;
                        static int current_audio = 0;
                        const size_t aSize = 30;
                        const char* AudioList[aSize];
                        std::vector<const char*> sList;
                        for (int i = 0; i < AudioArr.size(); i++) {
                            sList.push_back(AudioArr[i].mAudioPath.c_str());
                        }
                        std::copy(sList.begin(), sList.end(), AudioList);
                        ImGui::Text("Inserted Audio Files:");
                        ImGui::Combo("##Audio In Component", &current_audio, AudioList, static_cast<int>(sList.size()), static_cast<int>(sList.size()));
                        //edit channel group
                        if (AudioArr.size() > current_audio) {
                            ImGui::Text("Inserted Audio Channel Group:");
                            const char* ChList = "None\0Master\0BGM\0SFX";
                            int ChIndex = static_cast<int>(AudioComp[current_audio].mChannelGroup);
                            ImGui::Combo("###test", &ChIndex, ChList);
                            ImGui::PushItemWidth(100.0f);
                            AudioComp[current_audio].mChannelGroup = static_cast<Audio::AudioType>(ChIndex);
                        }
                        //option to add audio to array
                        if (ImGui::Button("Add Audio")) {
                            Audio::AudioPiece nAud;
                            nAud.mAudioPath = audioFileList[currentfile];
                            nAud.mChannelGroup = Audio::AudioType::NONE;
                            nAud.mChannel = 0;
                            nAud.is_Looping = false;
                            nAud.should_play = false;
                            nAud.should_stop = false;
                            nAud.is_Playing = false;
                            AudioArr.push_back(nAud);
                        }
                        ImGui::SameLine();
                        //option to remove audio from array
                        if (ImGui::Button("Remove Audio")) {
                            AudioArr.erase(AudioArr.begin() + current_audio);
                        }
                    }
                }
                //Tag Component
                if (p_ecs.HaveComponent<Tag>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("TAG", ImGuiTreeNodeFlags_None))
                    {
                        auto& tag = p_ecs.GetComponent<Tag>(selectedEntity).GetTag();
                        char buffer1[256];
                        memset(buffer1, 0, sizeof(buffer1));
                        strcpy_s(buffer1, sizeof(buffer1), tag.c_str());
                        if (ImGui::InputText("Tag", buffer1, sizeof(buffer1)))
                        {
                            tag = std::string(buffer1);
                        }
                    }
                }
                //HUD Component
                if (p_ecs.HaveComponent<HUDComponent>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("HUD", ImGuiTreeNodeFlags_None))
                    {
                        auto& HUDComp = p_ecs.GetComponent<HUDComponent>(selectedEntity);
                        const char* HUDList = "Static\0Boss Overlay\0Player Health Bar\0Boss Health Bar\0Block Bar\0Dash Bar\0Charge Attack\0Text";
                        auto HUDType = HUDComp.GetType();
                        int HUDIndex = static_cast<int>(HUDType);
                        ImGui::Combo("HudType", &HUDIndex, HUDList);
                        ImGui::PushItemWidth(100.0f);
                        HUDComp.SetType(static_cast<HUDComponent::ElementType>(HUDIndex));

                        auto& Offset = HUDComp.GetOffset();
                        ImGui::PushItemWidth(100.0f);
                        ImGui::Text("Offset"); ImGui::SameLine();
                        ImGui::Text("X"); ImGui::SameLine();                    //set a "x" to indicate x-axis
                        ImGui::DragFloat("##OffsetX", (float*)&Offset.x, 0.005f); ImGui::SameLine(); //char name , pass float pointer to position vec2D which hold x and y, the scaling value in imgui
                        ImGui::PushID(1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::DragFloat("##OffsetY", (float*)&Offset.y, 0.005f);
                        ImGui::PopID();
                        HUDComp.SetOffset(Offset);
                    }
                }
                //Enemy Attributes
                if (p_ecs.HaveComponent<EnemyAttributes>(selectedEntity))
                {
                    if (ImGui::CollapsingHeader("Enemy Attributes", ImGuiTreeNodeFlags_None))
                    {
                        auto& attrib = p_ecs.GetComponent<EnemyAttributes>(selectedEntity);

                        static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
                        int healthSlider = attrib.mHealth;
                        int maxHealthSlider = attrib.mMaxHealth;
                        int damageSlider = attrib.mDamage;
                        float attackSlider = attrib.mAttackTimer;
                        float damageCooldown = attrib.mDamageCoolDownTimer;
                        int enemyType = static_cast<int>(attrib.mEnemyType);
                        const char* EnemyList = "None\0Melee\0Ranged\0Special\0Boss";

                        ImGui::SliderInt("Health (0 -> 150)", &healthSlider, 0, 150, "%d", flags);
                        ImGui::SliderInt("Max Health (0 -> 200)", &maxHealthSlider, 0, 200, "%d", flags);
                        ImGui::SliderInt("Damage (0 -> 50)", &damageSlider, 0, 50, "%d", flags);
                        ImGui::SliderFloat("Attack Duration (0 -> 10)", &attackSlider, 0, 10, "%f", flags);
                        ImGui::SliderFloat("Attack Cooldown (0 -> 10)", &damageCooldown, 0, 10, "%f", flags);
                        ImGui::Combo("Enemy Type", &enemyType, EnemyList);
                        ImGui::PushItemWidth(100.0f);

                        attrib.mHealth = healthSlider;
                        attrib.mMaxHealth = maxHealthSlider;
                        attrib.mDamage = damageSlider;
                        attrib.mAttackTimer = attackSlider;
                        attrib.mDamageCoolDownTimer = damageCooldown;
                        attrib.mEnemyType = static_cast<EnemyAttributes::EnemyTypes>(enemyType);
                    }
                }


                if (ImGui::Button("Delete Component"))
                    ImGui::OpenPopup("Delete Component");

                if (ImGui::BeginPopup("Delete Component"))
                {
                    if (ImGui::MenuItem("Transform") && p_ecs.HaveComponent<Transform>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Transform>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Sprite") && p_ecs.HaveComponent<Sprite>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Sprite>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Collider") && p_ecs.HaveComponent<Collider>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Collider>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("RigidBody") && p_ecs.HaveComponent<RigidBody>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<RigidBody>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Logic") && p_ecs.HaveComponent<Logic>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Logic>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("PlayerAttributes") && p_ecs.HaveComponent<PlayerAttributes>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<PlayerAttributes>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Audio") && p_ecs.HaveComponent<Audio>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Audio>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Tag") && p_ecs.HaveComponent<Tag>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Tag>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("HUD") && p_ecs.HaveComponent<HUDComponent>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<HUDComponent>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("EnemyAttributes") && p_ecs.HaveComponent<EnemyAttributes>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<EnemyAttributes>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (ImGui::MenuItem("Attributes") && p_ecs.HaveComponent<Attributes>(selectedEntity))
                    {
                        p_ecs.RemoveComponent<Attributes>(selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        } //end of is_ShowWindow

    }



    /*!*************************************************************************
     Audio manager allows users to select and play, pause and test different
     audios in the editor
    ****************************************************************************/
    void LevelEditor::AudioEditor()
    {
        if (is_ShowWindow)
        {
            std::vector<const char*> audiofilenames;
            const size_t arraysize = 100;
            const char* audioFileList[arraysize];
            std::string audioPath = "Assets/metadigger/";
            static int currentfile = 0;

            for (auto& item : mAudioFile)
            {
                audiofilenames.push_back(item.c_str());
            }

            std::copy(audiofilenames.begin(), audiofilenames.end(), audioFileList);

            for (size_t i = audiofilenames.size(); i < arraysize; i++)
            {
                audioFileList[i] = "EMPTY";
            }

            ImGui::Begin("Audio Manager");

            ImGui::Combo("##LoadFile", &currentfile, audioFileList, static_cast<int>(audiofilenames.size()), static_cast<int>(audiofilenames.size()));

            static bool pauseButton = false;

            if (ImGui::Button("Play Audio"))
            {
                ImGui::SameLine();
                ImGui::Text("Playing!");
                current_sound = p_Audio->PlaySound(audioPath + mAudioFileList[currentfile].path().filename().string(), Audio::AudioType::MASTER);
            }

            for (auto i = p_Audio->mChannelMap.begin(); i != p_Audio->mChannelMap.end(); i++)
            {
                //print out text indicating channel number
                ImGui::Text("Playing Channel %s", std::to_string(i->first).c_str());

                if (ImGui::Button("Pause Audio"))
                {
                    p_Audio->PauseSound(i->first);
                }

                ImGui::SameLine();

                if (ImGui::Button("Unpause Audio"))
                {
                    p_Audio->UnpauseSound(i->first);
                }

                if (ImGui::Button("Stop Audio"))
                {
                    p_Audio->StopChannel(i->first);
                }

            }

            if (ImGui::Button("Delete Audio"))
            {
                std::filesystem::remove(mAudioFileList[currentfile].path());
                mAudioFileList.erase(mAudioFileList.begin() + currentfile);
                // used to load audio
                mAudioFile.erase(mAudioFile.begin() + currentfile);
            }

            static int item_current = 1;

            float f1 = p_Audio->GetVolumeByChannel(p_Audio->GetMasterChannelGroup());
            float f4 = p_Audio->GetVolumeByChannel(p_Audio->GetBGMChannelGroup());
            float f5 = p_Audio->GetVolumeByChannel(p_Audio->GetSFXChannelGroup());

            for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
            {
                if (p_ecs.HaveComponent<Audio>(i))
                {
                    f1 = p_ecs.GetComponent<Audio>(i).GetMasterVolume();
                    f4 = p_ecs.GetComponent<Audio>(i).GetBGMVolume();
                    f5 = p_ecs.GetComponent<Audio>(i).GetSFXVolume();
                }
            }
            //set voulume slider
            ImGui::SliderFloat("Master Volume", &f1, 0.0f, 1.0f, "Min - Max %.3f");
            p_Audio->SetVolumeByChannel(p_Audio->GetMasterChannelGroup(), f1);

            ImGui::SliderFloat("BGM vol", &f4, 0.0f, 1.0f, "Min - Max %.3f");
            if (f4 > f1)
            {
                f4 = f1;
            }
            p_Audio->SetVolumeByChannel(p_Audio->GetBGMChannelGroup(), f4);


            ImGui::SliderFloat("SFX vol", &f5, 0.0f, 1.0f, "Min - Max %.3f");
            if (f5 > f1)
            {
                f5 = f1;
            }
            p_Audio->SetVolumeByChannel(p_Audio->GetSFXChannelGroup(), f5);

            for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
            {
                if (p_ecs.HaveComponent<Audio>(i))
                {
                    p_ecs.GetComponent<Audio>(i).SetMasterVolume(f1);
                    p_ecs.GetComponent<Audio>(i).SetBGMVolume(f4);
                    p_ecs.GetComponent<Audio>(i).SetSFXVolume(f5);
                }
            }
            ImGui::End();
        }
    }

    /*!*************************************************************************
    using std::filesystem we insert the file path if audio assets into the editor
    ****************************************************************************/
    void LevelEditor::insertAudioFilePath(std::string in)
    {
        //create new directory entry
        //insert entry into mAudioFileList
        //insert path into mAudioFile
        auto const& dir_entry = std::filesystem::directory_entry{ in };
        mAudioFileList.emplace_back(dir_entry);
        mAudioFile.emplace_back(dir_entry.path().filename().string());
    }

    /*!*************************************************************************
   using std::filesystem we insert the file path into the system
   ****************************************************************************/
    void LevelEditor::insertTextureFilePath(std::string in)
    {
        auto const& dir_entry = std::filesystem::directory_iterator(in);
        for (const auto& entry : dir_entry)
        {
            if (entry.path().extension() == ".png") {

            }
        }
    }
}