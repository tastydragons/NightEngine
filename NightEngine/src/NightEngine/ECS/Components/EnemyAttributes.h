#pragma once

#include "IComponent.h"

#include "NightEngine/Math/Physics.h"

namespace Night
{
	class EnemyAttributes : public IComponent
	{
	public:

		enum class EnemyTypes
		{
			ENEMY_NONE,
			ENEMY_MELEE,
			ENEMY_RANGED,
			ENEMY_PICKUP,
			ENEMY_BOSS,
		};
		enum class Facing
		{
			RIGHT,
			LEFT
		};

		EnemyAttributes();
		~EnemyAttributes() = default;
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
		float mAttackTimer;
		float mAttackCoolDown;
		float mDamageCoolDownTimer;
		float mDamageDurationTimer;
		float mIdleTimer;
		float mRetreatDurationTimer;
		float mDeathTimer;
		float mFadeofftimer;
		int mHitCounter;
		bool mIsAlive;
		bool mIsDamaged;
		bool mIsChargedDamage;
		bool mIsPickedUp;
		EnemyTypes mEnemyType;
		Facing mEnemyFacing;
		vec2D mVel;
		entityPhysics mPhys;
	};
}