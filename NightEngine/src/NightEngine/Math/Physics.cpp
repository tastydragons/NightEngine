#include "nepch.h"

#include "Physics.h"
#include "NEMath.h"

namespace Night
{
    /*!*************************************************************************
    This function takes in the current velocity of the object and applies acceleration
    to it and multiplying said acceleration by the frame time to ensure consistent
    acceleration regardless of frame rate
    ****************************************************************************/
    vec2D entityPhysics::accelent(vec2D& entvel, vec2D applied, float dt) {
        entvel += applied * dt;
        entvel = entvel * 0.99f * dt;
        return entvel;
    }
    /*!*************************************************************************
    This function takes in the current velocity of the object and applies deceleration
    to it and multiplying said deceleration by the frame time to ensure consistent
    acceleration regardless of frame rate
    ****************************************************************************/
    vec2D entityPhysics::friction(vec2D& entvel, float dt) {
        entvel -= entvel * 15.5f * dt;
        return entvel;
    }
    /*!*************************************************************************
    This function takes in the current velocity of the object and applies gravity
    to it and multiplying said acceleration by the frame time to ensure consistent
    acceleration regardless of frame rate
    ****************************************************************************/
    vec2D entityPhysics::gravity(vec2D& entvel, float dt) {
        entvel.y -= 9.8f * dt;
        return entvel;
    }
    /*!*************************************************************************
    This function is a simple static circle to circle collision detection function
    that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleCircleCircle(vec2D ent1, vec2D ent2, float rad1, float rad2) {
        if (distance(ent1, ent2) <= (rad1 + rad2)) {
            return 1;
        }
        else {
            return 0;
        }
    }
    /*!*************************************************************************
    This function is a simple static circle to line collision detection function
    that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleCircleLine(vec2D ent1, float rad1, vec2D max1, vec2D min1) {
        vec2D lineNormal = vec2D();
        lineNormal.x = max1.y - min1.y;
        lineNormal.y = min1.x - max1.x;
        vec2D straightLine = ent1 - min1;
        float straightDist = dotProduct(straightLine, lineNormal);
        if (straightDist <= rad1) {
            return 1;
        }
        else {
            return 0;
        }
    }
    /*!*************************************************************************
    This function is a simple static circle to point collision detection function
    that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleCirclePoint(vec2D ent1, float rad1, vec2D pt1) {
        if (distance(ent1, pt1) <= rad1) {
            return 1;
        }
        else {
            return 0;
        }
    }
    /*!*************************************************************************
    This function is a simple static circle to AABB collision detection function
    that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleCircleRect(vec2D ent1, float rad1, vec2D max1, vec2D min1, vec2D center) {
        UNREFERENCED_PARAMETER(center);
        if (ent1.x + rad1 < min1.x || ent1.x - rad1 > max1.x || ent1.y + rad1 < min1.y || ent1.y - rad1 > max1.y) {
            return 0;
        }
        return 1;
    }
    /*!*************************************************************************
    This function is a simple static AABB to AABB collision detection function
    that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleRectRect(vec2D max1, vec2D min1, vec2D max2, vec2D min2) {

        if (!(min1.x > max2.x || min2.x > max1.x || max1.y < min2.y || max2.y < min1.y)) {
            return true;
        }
        else
            return false;
    }
    /*!*************************************************************************
    This function is a simple static circle to playable area AABB collision detection
    function that returns a boolean inditating whether collision was detected or not
    ****************************************************************************/
    int entityCollision::simpleBoxCircle(vec2D ent1, float rad1, vec2D min1, vec2D max1) {
        if ((ent1.x - min1.x) <= rad1 || (max1.x - ent1.x) <= rad1 || (ent1.y - min1.y) <= rad1 || (max1.y - ent1.y) <= rad1) {
            return 1;
        }
        else {
            return 0;
        }
    }
    /*!*************************************************************************
    This function is a possible function to declare in the event that the game
    requires the calculation of knockback upon some event.
    ****************************************************************************/
    void Knockback()
    {

    }
}