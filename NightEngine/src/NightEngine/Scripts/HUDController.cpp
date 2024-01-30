#include "hzpch.h"

#include "HUDController.h"

namespace Night
{
    /*!*************************************************************************
    Default constructor for HUD Controller
    ****************************************************************************/
    HUDController::HUDController() : mEntityID{ 0 } {}
    /*!*************************************************************************
    Returns a new copy of HUDController Script
    ****************************************************************************/
    HUDController* HUDController::Clone() const
    {
        return new HUDController(*this);
    }
    /*!*************************************************************************
    Start State of HUDController Script
    ****************************************************************************/
    void HUDController::Start()
    {
        for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
        {
            if (p_ecs.HaveComponent<PlayerAttributes>(i))
            {
                mEntityID = i;
            }
        }
    }
    /*!*************************************************************************
    Update Loop of HUDController Script
    ****************************************************************************/
    void HUDController::Update(float Frametime)
    {
        UNREFERENCED_PARAMETER(Frametime);

        if (p_ecs.HaveComponent<HUDComponent>(GetScriptEntityID()) && p_ecs.HaveComponent<Transform>(GetScriptEntityID()) && p_ecs.HaveComponent<Sprite>(GetScriptEntityID()))
        {
            auto& pComp = p_ecs.GetComponent<HUDComponent>(GetScriptEntityID());
            auto& pTrans = p_ecs.GetComponent<Transform>(GetScriptEntityID());

            //get camera position
            vec2D camPos = vec2D(Graphic::camera.GetPosition().x, Graphic::camera.GetPosition().y);
            //update position
            pTrans.SetPos(camPos + pComp.GetOffset());
            if (pComp.GetType() == HUDComponent::ElementType::Static) {
                pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x), camPos.y + pComp.GetOffset().y);
            }
            if (pComp.GetType() == HUDComponent::ElementType::BossStatic) {
                for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
                {
                    if (p_ecs.HaveComponent<EnemyAttributes>(i) && p_ecs.HaveComponent<NameTag>(i) && p_ecs.GetComponent<NameTag>(i).GetNameTag() == "Boss")
                    {
                        pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x), camPos.y + pComp.GetOffset().y);
                        if (p_ecs.GetComponent<EnemyAttributes>(i).mIsAlive == true) {
                            p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetLayerOrder(7);
                        }
                        else {
                            p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetLayerOrder(8);
                        }
                    }
                }
            }
            if (pComp.GetType() == HUDComponent::ElementType::HealthBar) {
                //update scale
                pTrans.SetScale((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mHealth) / 2.0f / (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mMaxHealth) / 2.0f, pTrans.GetScale().y);
                pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x) + ((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mHealth) / 2.0f / (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mMaxHealth) / 2.0f) / 2.5f, camPos.y + pComp.GetOffset().y);
            }
            if (pComp.GetType() == HUDComponent::ElementType::BossHP) {
                //update scale
                for (Entity i = 0; i < p_ecs.GetTotalEntities(); ++i)
                {
                    if (p_ecs.HaveComponent<EnemyAttributes>(i) && p_ecs.HaveComponent<NameTag>(i) && p_ecs.GetComponent<NameTag>(i).GetNameTag() == "Boss")
                    {
                        if (p_ecs.GetComponent<EnemyAttributes>(i).mIsAlive == true) {
                            p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetLayerOrder(7);
                        }
                        else {
                            p_ecs.GetComponent<Sprite>(GetScriptEntityID()).SetLayerOrder(8);
                        }
                        pTrans.SetScale(((float)p_ecs.GetComponent<EnemyAttributes>(i).mHealth) / ((float)p_ecs.GetComponent<EnemyAttributes>(i).mMaxHealth) / 1.5f, pTrans.GetScale().y);
                        pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x) + (((float)p_ecs.GetComponent<EnemyAttributes>(i).mHealth) / ((float)p_ecs.GetComponent<EnemyAttributes>(i).mMaxHealth) / 1.5f / 2.f), camPos.y + pComp.GetOffset().y);
                    }
                }


            }
            if (pComp.GetType() == HUDComponent::ElementType::BlockBar) {
                //check for timing of cooldown
                //update alpha
                pTrans.SetScale((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown) / 7.0f
                    - (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown) / 7.0f, pTrans.GetScale().y);
                pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x) - ((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown / 100.f) / 2.0f /
                    (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mBlockMaxCoolDown / 100.f) / 2.0f) / 2.5f, camPos.y + pComp.GetOffset().y);
            }
            if (pComp.GetType() == HUDComponent::ElementType::DashBar) {
                //check for timing of cooldown
                pTrans.SetScale((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown) / 5.5f
                    - (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown) / 5.5f, pTrans.GetScale().y);
                pTrans.SetPos(static_cast<float>(camPos.x + pComp.GetOffset().x) - ((p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown / 100.f) / 2.0f /
                    (p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown / p_ecs.GetComponent<PlayerAttributes>(mEntityID).mDashMaxCoolDown / 100.f) / 2.0f) / 2.5f, camPos.y + pComp.GetOffset().y);
            }
        }
    }
    /*!*************************************************************************
    End State for HUDController
    ****************************************************************************/
    void HUDController::End()
    {
        delete this;
    }
    /*!*************************************************************************
    Returns the name of Script
    ****************************************************************************/
    std::string HUDController::GetScriptName()
    {
        return "HUDController";
    }
}