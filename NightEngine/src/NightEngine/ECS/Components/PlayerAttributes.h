#pragma once

#include "IComponent.h"

#include "NightEngine/Math/Physics.h"

namespace Night
{
	class PlayerAttributes : public IComponent
	{
	public:
		enum class Facing
		{
			NONE,
			RIGHT,
			LEFT
		};
		PlayerAttributes();
		~PlayerAttributes() = default;
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const;

		/*!*************************************************************************
		Retrieves Component Entity ID
		****************************************************************************/
		Entity& GetComponentEntityID() { return entityID; }
		/*!*************************************************************************
		Sets Components Entity ID
		****************************************************************************/
		void SetComponentEntityID(Entity& entity) { entityID = entity; }

	public:
		int mHealth;
		int mMaxHealth;
		int mDamage;
		int mAttackCounter;
		float mDamageCoolDown;
		float mDashCoolDown;
		float mDashMaxCoolDown;
		float mDashDurationTimer;
		float mBlockCoolDown;	//5.0f
		float mBlockMaxCoolDown;
		float mBlockDurationTimer;
		float mCooldownTimer;
		float mCooldownMaxTimer;
		float mDamageTaken;
		float mDamageDurationTimer;
		float mHitStopTimer;
		float mChargeAttackCoolDown;
		float mChargeTimer;
		float mDeathDuration;
		bool mIsChargeAttack;
		bool mIsDamaged;
		bool mIsBlocking;
		bool mIsDashing;
		bool mIsGOD;
		bool mComboNext;
		vec2D mVel;
		vec2D mDir;
		Facing mFacing;
		entityPhysics mPhys;
	};
}