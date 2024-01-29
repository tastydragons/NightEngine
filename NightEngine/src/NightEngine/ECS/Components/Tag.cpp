#include "hzpch.h"

#include "Tag.h"

namespace Night
{
	/*!*************************************************************************
	Ctor for Tag Component
	****************************************************************************/
	Tag::Tag() : mTag{ "Untagged" }, mTargetTag{ "" } {}

	/*!*************************************************************************
	Deserialize for Tag Component
	****************************************************************************/
	bool Tag::Deserialize(const rapidjson::Value& obj)
	{
		mTag = obj["Tag"].GetString();
		mTargetTag = obj["TargetTag"].GetString();
		return true;
	}

	/*!*************************************************************************
	Serialize for Tag Component
	****************************************************************************/
	bool Tag::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("Tag");
		writer->String(mTag.c_str());
		writer->Key("TargetTag");
		writer->String(mTargetTag.c_str());
		return true;
	}
}