#include "hzpch.h"

#include "Collider.h"

namespace Night
{
	/*!*************************************************************************
	This is the constructor for the collider component to initialize all data members
	Data members will be changed during the de-serialization function call if the
	specific entity has serialized collider information
	****************************************************************************/
	Collider::Collider() {
		for (int i = 0; i < 2; ++i)
		{
			mColArr[i].mCol = ColliderType::none;
			mColArr[i].mCollisionNormal = vec2D();
			mColArr[i].mMin = vec2D();
			mColArr[i].mMax = vec2D();
			mColArr[i].mOffset = vec2D();
			mColArr[i].mHit = 0;
			mColArr[i].mRadius = 0;
			mColArr[i].is_Alive = true;
		}
	}
	/*!*************************************************************************
	This function de-serializes the level colliders from the given level json file
	****************************************************************************/
	bool Collider::Deserialize(const rapidjson::Value& obj)
	{
		mColArr[0].mCol = static_cast<ColliderType>(obj["ColliderType1"].GetInt());
		mColArr[1].mCol = static_cast<ColliderType>(obj["ColliderType2"].GetInt());
		mColArr[0].mMin = vec2D(obj["minX1"].GetFloat(), obj["minY1"].GetFloat());
		mColArr[1].mMin = vec2D(obj["minX2"].GetFloat(), obj["minY2"].GetFloat());
		mColArr[0].mMax = vec2D(obj["maxX1"].GetFloat(), obj["maxY1"].GetFloat());
		mColArr[1].mMax = vec2D(obj["maxX2"].GetFloat(), obj["maxY2"].GetFloat());
		mColArr[0].mOffset = vec2D(obj["offsetX1"].GetFloat(), obj["offsetY1"].GetFloat());
		mColArr[1].mOffset = vec2D(obj["offsetX2"].GetFloat(), obj["offsetY2"].GetFloat());
		mColArr[0].mRadius = obj["radius1"].GetFloat();
		mColArr[1].mRadius = obj["radius2"].GetFloat();
		return true;
	}
	/*!*************************************************************************
	This function saves the level information by serializing it to the level json
	file
	****************************************************************************/
	bool Collider::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("ColliderType1");
		writer->Int(static_cast<int>(mColArr[0].mCol));
		writer->Key("ColliderType2");
		writer->Int(static_cast<int>(mColArr[1].mCol));
		writer->Key("minX1");
		writer->Double(mColArr[0].mMin.x);
		writer->Key("minX2");
		writer->Double(mColArr[1].mMin.x);
		writer->Key("minY1");
		writer->Double(mColArr[0].mMin.y);
		writer->Key("minY2");
		writer->Double(mColArr[1].mMin.y);
		writer->Key("maxX1");
		writer->Double(mColArr[0].mMax.x);
		writer->Key("maxX2");
		writer->Double(mColArr[1].mMax.x);
		writer->Key("maxY1");
		writer->Double(mColArr[0].mMax.y);
		writer->Key("maxY2");
		writer->Double(mColArr[1].mMax.y);
		writer->Key("offsetX1");
		writer->Double(mColArr[0].mOffset.x);
		writer->Key("offsetX2");
		writer->Double(mColArr[1].mOffset.x);
		writer->Key("offsetY1");
		writer->Double(mColArr[0].mOffset.y);
		writer->Key("offsetY2");
		writer->Double(mColArr[1].mOffset.y);
		writer->Key("radius1");
		writer->Double(mColArr[0].mRadius);
		writer->Key("radius2");
		writer->Double(mColArr[1].mRadius);
		return true;
	}
}