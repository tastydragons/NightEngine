#pragma once

namespace Night
{
    class entityPhysics {
    public:
        /*!*************************************************************************
        This function takes in the current velocity of the object and applies acceleration
        to it and multiplying said acceleration by the frame time to ensure consistent
        acceleration regardless of frame rate
        ****************************************************************************/
        vec2D accelent(vec2D& entvel, vec2D applied, float dt);
        /*!*************************************************************************
        This function takes in the current velocity of the object and applies deceleration
        to it and multiplying said deceleration by the frame time to ensure consistent
        acceleration regardless of frame rate
        ****************************************************************************/
        vec2D friction(vec2D& entvel, float dt);
        /*!*************************************************************************
        This function takes in the current velocity of the object and applies gravity
        to it and multiplying said acceleration by the frame time to ensure consistent
        acceleration regardless of frame rate
        ****************************************************************************/
        vec2D gravity(vec2D& entvel, float dt);
    };

    class entityCollision {
    public:
        entityCollision() = default;

    public:
        /*!*************************************************************************
        This function is a simple static circle to circle collision detection function
        that returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleCircleCircle(vec2D ent1, vec2D ent2, float rad1, float rad2);
        /*!*************************************************************************
        This function is a simple static circle to line collision detection function
        that returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleCircleLine(vec2D ent1, float rad1, vec2D max1, vec2D min1);
        /*!*************************************************************************
        This function is a simple static circle to point collision detection function
        that returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleCirclePoint(vec2D ent1, float rad1, vec2D pt1);
        /*!*************************************************************************
        This function is a simple static circle to AABB collision detection function
        that  returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleCircleRect(vec2D ent1, float rad1, vec2D max1, vec2D min1, vec2D center);
        /*!*************************************************************************
        This function is a simple static AABB to AABB collision detection function
        that returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleRectRect(vec2D max1, vec2D min1, vec2D max2, vec2D min2);
        /*!*************************************************************************
        This function is a simple static circle to playable area AABB collision detection
        function that returns a boolean inditating whether collision was detected or not
        ****************************************************************************/
        int simpleBoxCircle(vec2D ent1, float rad1, vec2D min1, vec2D max1);
    };
}