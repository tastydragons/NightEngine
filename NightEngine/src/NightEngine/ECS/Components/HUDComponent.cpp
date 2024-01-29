#include "hzpch.h"

#include "HUDComponent.h"

namespace Night
{
	/*!*************************************************************************
	Constructor for HUD component
	****************************************************************************/
	HUDComponent::HUDComponent() : mType{ ElementType::Static }, offset{ vec2D() } {}

	/*!*************************************************************************
	Derserialize HUD component
	****************************************************************************/
	bool HUDComponent::Deserialize(const rapidjson::Value& obj)
	{
		mType = static_cast<ElementType>(obj["ElementType"].GetInt());
		offset = vec2D(obj["offsetX"].GetFloat(), obj["offsetY"].GetFloat());
		return true;
	}

	/*!*************************************************************************
	Serialize HUD component
	****************************************************************************/
	bool HUDComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		writer->Key("ElementType");
		writer->Int(static_cast<int>(mType));
		writer->Key("offsetX");
		writer->Double(offset.x);
		writer->Key("offsetY");
		writer->Double(offset.y);
		return true;
	}
}