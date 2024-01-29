#include "hzpch.h"

#include "PlayerAttributes.h"

namespace Night
{
	PlayerAttributes::PlayerAttributes() : mHealth{ 100 }, mMaxHealth{ 100 }, mDamage{ 20 }, mAttackCounter{ 0 }, mDamageCoolDown{ 0.0f }, mDashCoolDown{ 0.0f }, mDashMaxCoolDown{ 1.0f },
		mDashDurationTimer{ 0.2f }, mBlockCoolDown{ 0.0f }, mBlockMaxCoolDown{ 1.0f }, mBlockDurationTimer{ 2.0f }, mCooldownTimer{ 0.0f }, mCooldownMaxTimer{ 0.5f }, mDamageTaken{ 0.0f }, mDamageDurationTimer{ 0.5f },
		mHitStopTimer{ 0.0f }, mChargeAttackCoolDown{ 0.0f }, mChargeTimer{ 0.5f }, mDeathDuration{ 0.0f }, mIsChargeAttack{ false }, mIsDamaged{ false }, mIsBlocking{ false }, mIsDashing{ false }, mIsGOD{ false },
		mComboNext{ false }, mVel{ vec2D(1.0f,1.0f) }, mDir{ vec2D(1.0f,0.0f) }, mFacing{ Facing::NONE } {}

	/*!*************************************************************************
	Deserialize attributes
	****************************************************************************/
	bool PlayerAttributes::Deserialize(const rapidjson::Value& obj)
	{
		mHealth = obj["Health"].GetInt();
		mMaxHealth = obj["Max Health"].GetInt();
		mDamage = obj["Damage"].GetInt();
		return true;
	}

	/*!*************************************************************************
	Serialize attributes
	****************************************************************************/
	bool PlayerAttributes::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("Health");
		writer->Int(mHealth);
		writer->Key("Max Health");
		writer->Int(mMaxHealth);
		writer->Key("Damage");
		writer->Int(mDamage);
		return true;
	}
}