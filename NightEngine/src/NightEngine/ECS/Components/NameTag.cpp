#include "hzpch.h"

#include "NameTag.h"

namespace Night
{
	/*!*************************************************************************
	Ctor for NameTag
	****************************************************************************/
	NameTag::NameTag() : mName{ "none" } {}

	/*!*************************************************************************
	Deserialize NameTag Component
	****************************************************************************/
	bool NameTag::Deserialize(const rapidjson::Value& obj)
	{
		mName = obj["NameTag"].GetString();
		return true;
	}

	/*!*************************************************************************
	Serialize NameTag Component
	****************************************************************************/
	bool NameTag::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("NameTag");
		writer->String(mName.c_str());
		return true;
	}
}