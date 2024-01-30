#include "hzpch.h"

#include "Graphics.h"

#include "NightEngine/GUI/GUI.h"
#include <glm/gtc/matrix_transform.hpp>
#include "NightEngine/Core/Input.h"
#include "NightEngine/ResourceManager/ResourceManager.h"
#include "NightEngine/Timer/Timer.h"
#include "Platform/LevelEditor/LevelEditor.h"
#include "NightEngine/Math/NEMath.h"

#include "FrameBuffer.h"
#include "NightEngine/Timer/FPS.h"
#include "NightEngine/ECS/SceneManager.h"
#include "Platform/Logic/States/BossStates/BossDeath.h"

namespace Night
{
	/*!*************************************************************************
	Load Icon from filepath using resource manager
	****************************************************************************/
	void Graphic::LoadIconsTexture(std::string filename)
	{
		std::ifstream ifs(filename.c_str());
		std::string name, textPath;
		while (ifs >> name >> textPath)
			ResourceManager::LoadIcons(name.c_str(), textPath.c_str());

		ifs.close();
	}

	/*!*************************************************************************
	Init loop for Graphics
	****************************************************************************/
	void Graphic::Init()
	{

		ResourceManager::LoadShader("QuadShader", "Assets/Shaders/texture.shader");
		ResourceManager::LoadShader("LineShader", "Assets/Shaders/Line.shader");
		ResourceManager::LoadShader("CircleShader", "Assets/Shaders/Circle.shader");
		ResourceManager::LoadShader("ImpactShader", "Assets/Shaders/Impact.shader");

		LoadIconsTexture("Assets/Text/Icons.txt");
		std::string path = { "Assets/Textures/" };
		std::string folderpath[] = { "Characters", "Environment", "Gate", "UI", "Hazards" ,"CutScene", "Dialogues" };

		for (const auto& i : folderpath)
		{
			std::filesystem::path currentPath = path + i;
			auto currentFolders = std::filesystem::directory_iterator(currentPath);
			for (const auto& entry : currentFolders)
			{
				std::string gs = entry.path().generic_string();
				std::size_t pos = gs.find_last_of("/");
				std::size_t pos1 = gs.find(".");
				std::string Imagename = gs.substr(pos + 1, pos1 - pos - 1);
				ResourceManager::LoadTexture(Imagename, gs);
			}
		}
		FrameBufferSpecification fbspec;
		fbspec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::DEPTH };
		fbspec.Width = mWinData.GetWidth();
		fbspec.Height = mWinData.GetHeight();
		p_FrameBuffer->SetFrameBufferSpecification(fbspec);
		Renderer::Init();
		mFont->Init();
	}

	/*!*************************************************************************
	Update loop for Graphics
	****************************************************************************/
	void Graphic::Update(float frametime)
	{
		Timer::GetInstance().Start(Systems::GRAPHIC);
		Timer::GetInstance().GetDT(Systems::GRAPHIC);

		if (p_Editor->is_ShowWindow)
			mcamera = &scene_camera;
		else
		{
			glViewport(0, 0, p_Editor->GetWindow()->Getter().m_Width, p_Editor->GetWindow()->Getter().m_Height);
			Night::Graphic::mcamera->Resize(static_cast<float>(p_Editor->GetWindow()->Getter().m_Width), static_cast<float>(p_Editor->GetWindow()->Getter().m_Height));

			mcamera = &camera;
			if (!p_Editor->is_ShowWindow)
			{
				glViewport(0, 0, p_Editor->GetWindow()->Getter().m_Width, p_Editor->GetWindow()->Getter().m_Height);
				mcamera->Resize(static_cast<float>(p_Editor->GetWindow()->Getter().m_Width), static_cast<float>(p_Editor->GetWindow()->Getter().m_Height));
			}
		}
		//Resize
		if (FrameBufferSpecification spec = p_FrameBuffer->GetSpecification();
			p_Editor->mViewportSize.x > 0.0f && p_Editor->mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != p_Editor->mViewportSize.x || spec.Height != p_Editor->mViewportSize.y) &&
			p_Editor->is_ShowWindow)
		{

			p_FrameBuffer->Resize((uint32_t)p_Editor->mViewportSize.x, (uint32_t)p_Editor->mViewportSize.y);
			mcamera->Resize(p_Editor->mViewportSize.x, p_Editor->mViewportSize.y);

		}


		mRenderer->ResetInfo();
		if (p_Editor->is_ShowWindow)
		{
			p_FrameBuffer->Bind();
		}

		mRenderer->SetClearColor({ 0.0f, 0.1f, 0.1f, 1.0f });
		mRenderer->Clear();
		int value = -1;
		glClearTexImage(p_FrameBuffer->GetColorAttachmentRendererID(1), 0, GL_RGBA8, GL_INT, &value);

		mRenderer->Begin(*mcamera);// begin of the renderer 


		p_GUI->VPmat = mcamera->GetViewProjectionMatrix();


		for (auto layer = 0; layer < 8; layer++)
		{
			for (auto i = mEntities.begin(); i != mEntities.end(); i++)
			{

				auto& transform = p_ecs.GetComponent<Transform>(*i);
				auto& sprite = p_ecs.GetComponent<Sprite>(*i);
				if (sprite.LayerOrder == layer)
				{//render the lowest layer level then follow on
					if (sprite.is_Animated)
					{
						mAimator.UpdateAnimation(frametime, p_ecs.GetComponent<Sprite>(*i));
					}
					if (sprite.is_SpriteSheet)
					{

						mIndex1 = SpriteRender::CreateSprite(GETTEXTURE(sprite.GetTexture()), { sprite.GetIndex().x, sprite.GetIndex().y },
							{ sprite.GetUVCoor().x, sprite.GetUVCoor().y });

						mRenderer->DrawSprite({ transform.GetPos().x , transform.GetPos().y }, { transform.GetScale().x , transform.GetScale().y },
							transform.GetRot(), mIndex1);
					}
					else
					{
						mRenderer->DrawQuad({ transform.GetPos().x, transform.GetPos().y }, { transform.GetScale().x, transform.GetScale().y },
							transform.GetRot(), GETTEXTURE(sprite.GetTexture()));
					}
					if (p_Editor->mDebugDraw)
					{
						if (p_ecs.HaveComponent<Collider>(*i) && ((p_ecs.GetComponent<Collider>(*i)[0].is_Alive) || (p_ecs.GetComponent<Collider>(*i)[1].is_Alive))) {
							for (int u = 0; u < 2; u++) {
								if (p_ecs.GetComponent<Collider>(*i)[u].mCol == Collider::ColliderType::rect || p_ecs.GetComponent<Collider>(*i)[u].mCol == Collider::ColliderType::box)
								{
									auto& collider = p_ecs.GetComponent<Collider>(*i);
									mRenderer->DrawRect({ transform.GetPos().x + collider[u].mOffset.x + (collider[u].mMax.x / 2) - (collider[u].mMin.x / 2) , transform.GetPos().y + collider[u].mOffset.y + (collider[u].mMax.y / 2) - (collider[u].mMin.y / 2), 0.0f },
										{ collider[u].mMin.x + collider[u].mMax.x , collider[u].mMin.y + collider[u].mMax.y },
										{ 1.0f, 0.0f, 0.0f,1.0f });
								}
								if (p_ecs.GetComponent<Collider>(*i)[u].mCol == Collider::ColliderType::circle || p_ecs.GetComponent<Collider>(*i)[u].mCol == Collider::ColliderType::bubble
									|| p_ecs.GetComponent<Collider>(*i)[u].mCol == Collider::ColliderType::bossball) {
									auto& collider = p_ecs.GetComponent<Collider>(*i);
									Night::Matrix4x4 translate = Night::Translate4x4(translate, transform.GetPos().x + collider[u].mOffset.x, transform.GetPos().y + collider[u].mOffset.y, 0.0f);
									Night::Matrix4x4 scale = Night::Scale4x4(scale, collider[u].mRadius * 2, collider[u].mRadius * 2, collider[u].mRadius * 2);
									Night::Matrix4x4 Transform = translate * scale;
									mRenderer->DrawCircle(Transform, { 0.5f,0.4f,1.0f, 1.0f }, 0.01f);
								}
							}
						}
					}
				}
			}
		}


		for (auto const& entity : mEntities)
		{
			if (p_ecs.HaveComponent<NameTag>(entity) && !p_Editor->is_ShowWindow)
			{
				if (p_ecs.GetComponent<NameTag>(entity).GetNameTag() == "player" && p_GUI->check_pause() == false)
				{
					camera.SetPosition({ p_ecs.GetComponent<Transform>(entity).GetPos().x,
						p_ecs.GetComponent<Transform>(entity).GetPos().y,
						0.0f });

					mcamera->SetZoomLevel(0.25f);
				}
				if (p_ecs.HaveComponent<PlayerAttributes>(entity))
				{
					if (p_ecs.GetComponent<PlayerAttributes>(entity).mDamageCoolDown > 0.0f && p_ecs.GetComponent<PlayerAttributes>(entity).mIsBlocking == false)
					{
						mRenderer->DrawQuadImpact({ p_ecs.GetComponent<Transform>(entity).GetPos().x,p_ecs.GetComponent<Transform>(entity).GetPos().y,0.0f },
							{ 1.f,1.f }, { 1.0f,0.0f,0.0f,p_ecs.GetComponent<PlayerAttributes>(entity).mDamageCoolDown * 4.f });
					}
				}

			}
			if (p_ecs.HaveComponent<NameTag>(entity) && p_ecs.GetComponent<NameTag>(entity).GetNameTag() == "Splash Screen")
			{
				p_ecs.GetComponent<Transform>(entity).GetScale().x += frametime;
				p_ecs.GetComponent<Transform>(entity).GetScale().y += frametime;

			}
			//for rendering of enemy health bar
			if (p_ecs.HaveComponent<EnemyAttributes>(entity) && p_ecs.HaveComponent<HUDComponent>(entity) && p_ecs.HaveComponent<Tag>(entity) && p_ecs.GetComponent<Tag>(entity).GetTag() == "Enemy") {
				if (p_ecs.GetComponent<EnemyAttributes>(entity).mIsAlive == true && p_ecs.GetComponent<EnemyAttributes>(entity).mEnemyType != EnemyAttributes::EnemyTypes::ENEMY_BOSS)
				{

					auto& mTrans = p_ecs.GetComponent<Transform>(entity);
					auto& mAtt = p_ecs.GetComponent<EnemyAttributes>(entity);
					auto& mHUD = p_ecs.GetComponent<HUDComponent>(entity);
					if (mTrans.GetScale().x < 0) {
						vec2D HPpos = vec2D(mTrans.GetPos().x + mHUD.GetOffset().x + ((float)(mAtt.mHealth) / (float)(mAtt.mMaxHealth) * -mTrans.GetScale().x / 2.5f / 2.0f), mTrans.GetPos().y + mHUD.GetOffset().y);
						vec2D HPScale = vec2D((float)(mAtt.mHealth) / (float)(mAtt.mMaxHealth) * -mTrans.GetScale().x / 2.5f, mTrans.GetScale().y / 25.0f);
						if (mAtt.mHealth > 0) {
							mRenderer->DrawQuad(HPpos, HPScale, 0.0f, { 1.0f, 0.f, 0.f, 1.f });
						}
						mRenderer->DrawQuad({ mTrans.GetPos().x , mTrans.GetPos().y + mHUD.GetOffset().y }, { 0.135f, 0.020f },
							mTrans.GetRot(), GETTEXTURE("EnemyHealthBar"));


					}
					else {
						vec2D HPpos = vec2D(mTrans.GetPos().x + mHUD.GetOffset().x + ((float)(mAtt.mHealth) / (float)(mAtt.mMaxHealth) * mTrans.GetScale().x / 2.5f / 2.0f), mTrans.GetPos().y + mHUD.GetOffset().y);
						vec2D HPScale = vec2D((float)(mAtt.mHealth) / (float)(mAtt.mMaxHealth) * mTrans.GetScale().x / 2.5f, mTrans.GetScale().y / 25.0f);
						if (mAtt.mHealth > 0) {
							mRenderer->DrawQuad(HPpos, HPScale, 0.0f, { 1.0f, 0.0f, 0.0f, 1.f });
						}
						mRenderer->DrawQuad({ mTrans.GetPos().x , mTrans.GetPos().y + mHUD.GetOffset().y }, { 0.135f, 0.020f },
							mTrans.GetRot(), GETTEXTURE("EnemyHealthBar"));
					}


				}
			}
			if (p_ecs.HaveComponent<EnemyAttributes>(entity) && p_ecs.GetComponent<EnemyAttributes>(entity).mEnemyType == EnemyAttributes::EnemyTypes::ENEMY_BOSS)
			{

				if (p_ecs.GetComponent<EnemyAttributes>(entity).mHealth <= 0 && p_ecs.GetComponent<EnemyAttributes>(entity).mDeathTimer > 3.f)
				{
					mRenderer->DrawQuadImpact({ camera.GetPosition().x ,camera.GetPosition().y,0.0f },
						{ 1.0f + p_ecs.GetComponent<EnemyAttributes>(entity).mFadeofftimer, 1.0f + p_ecs.GetComponent<EnemyAttributes>(entity).mFadeofftimer },
						{ 0.0f,0.0f,0.0f, p_ecs.GetComponent<EnemyAttributes>(entity).mFadeofftimer * 2.5f });
				}
				else if (p_ecs.GetComponent<EnemyAttributes>(entity).mHealth <= 0 && p_ecs.GetComponent<EnemyAttributes>(entity).mDeathTimer > 2.0f)
					mRenderer->DrawQuad({ 8.0f, 1.0f }, { 0.8f, 0.3f },
						{ 0.0f }, GETTEXTURE("Ending_Dialogue"));
			}
		}



		if (p_GUI->check_pause() == true && p_GUI->Check_menu() == false)
		{
			//UI background
			mRenderer->DrawQuad({ camera.GetPosition().x, camera.GetPosition().y }, { 3.52f, 1.89f }, 0.0f, GETTEXTURE("EndGameUI"));
			mRenderer->DrawQuad({ camera.GetPosition().x + -0.89f, camera.GetPosition().y + -0.01f }, { 1.21f, 1.54f }, 0.0f, GETTEXTURE("Avatar"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.522f, camera.GetPosition().y + 0.118f }, { 1.125f, 1.222f }, 0.0f, GETTEXTURE("MenuPanel"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.526f,camera.GetPosition().y + 0.361f }, { 0.802f, 0.123f }, 0.0f, GETTEXTURE("ResumeButton"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.526f, camera.GetPosition().y + 0.186f }, { 0.802f, 0.123f }, 0.0f, GETTEXTURE("ResumeButton"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.526f, camera.GetPosition().y + -0.007f }, { 0.802f, 0.123f }, 0.0f, GETTEXTURE("ResumeButton"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.526f, camera.GetPosition().y + -0.198f }, { 0.802f, 0.123f }, 0.0f, GETTEXTURE("ResumeButton"));
			mRenderer->DrawQuad({ camera.GetPosition().x + 0.526f, camera.GetPosition().y + -0.556f }, { 1.350f, 0.175f }, 0.0f, GETTEXTURE("ResumeButton"));

		}
		mRenderer->End();

		p_FrameBuffer->UnBind();


		mcamera->MouseScrolling();
		if (p_Input->KeyPressed(GLFW_KEY_F2) && !p_Editor->is_ShowWindow)
		{
			misFps = !misFps;
		}
		if (misFps)
		{
			auto i = std::to_string(FPS::GetInstance().GetFps());
			mFont->RenderText("FPS: " + i, { camera.GetPosition().x + 0.2f, camera.GetPosition().y + 0.2f },
				0.0008f, camera, { 0.87f, 0.92f, 0.18f });
		}
		Timer::GetInstance().Update(Systems::GRAPHIC);


	}

	/*!*************************************************************************
	End loop for Graphics
	****************************************************************************/
	void Graphic::End()
	{
		ResourceManager::clear();
		Renderer::ShutDown();
	}
}