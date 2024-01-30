#pragma once

#include "NightEngine/Math/Physics.h"

#include "NightEngine/ECS/Components/Components.h"

#include "Platform/System/System.h"

namespace Night
{
    class CollisionSystem : public System
    {
    public:
        /*!*************************************************************************
        This function returns the purpose of the system as a flag for when it is called
        ****************************************************************************/
        virtual std::string GetName() { return "Collision"; }
        /*!*************************************************************************
        This function initialises the system. As there are no data members that require
        initialization, this function is empty
        ****************************************************************************/
        virtual void Init() override;
        /*!*************************************************************************
        This function runs the logic of the system. It checks the list of entities with
        a collision component in pairs to see which of them are colliding. If two objects
        are colliding, collision response is dictated by the type of collider the object
        has
        ****************************************************************************/
        virtual void Update(float dt) override;
        /*!*************************************************************************
        This function ends the system. As there are no data members that require
        initialization, there are no data members that need to be un-initialised.
        Therefore this function is empty
        ****************************************************************************/
        virtual void End() override;
    private:
        entityCollision ecm;
    };
}