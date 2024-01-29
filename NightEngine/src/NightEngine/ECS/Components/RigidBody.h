#pragma once

#include "IComponent.h"

#include "NightEngine/Math/NEMath.h"

namespace Night
{
	class RigidBody : public IComponent
	{
	public:
		RigidBody();
		~RigidBody() = default;
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		vec2D& GetAccel() { return mAccel; }
		vec2D& GetVel() { return mVelocity; }
		vec2D& GetDir() { return mDirection; }
		vec2D& GetNextPos() { return mNextPos; }
		float& GetFriction() { return mFriction; }
		float& GetRestitution() { return mRestitution; }

		void SetAccel(vec2D vel) { mAccel = vel; }
		void SetVel(vec2D vel) { mVelocity = vel; }
		void SetDir(vec2D dir) { mDirection = dir; }
		void SetNextPos(vec2D vec) { mNextPos = vec; }
		void SetVel(float velX, float velY) { mVelocity = vec2D(velX, velY); }
		void SetDir(float dirX, float dirY) { mDirection = vec2D(dirX, dirY); }
		void SetFriction(float value) { mFriction = value; }
		void SetRestitution(float value) { mRestitution = value; }

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

	private:
		vec2D mAccel;
		vec2D mVelocity;
		vec2D mDirection;
		vec2D mNextPos;
		float mFriction;
		float mRestitution;
	};
}