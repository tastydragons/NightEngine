#include "hzpch.h"

#include "CameraComponent.h"

namespace Night
{
	/*!*************************************************************************
	Constructor for camera component
	****************************************************************************/
	CameraComponent::CameraComponent()
	{
	}

	/*!*************************************************************************
	Derserialize camera component
	****************************************************************************/
	bool CameraComponent::Deserialize(const rapidjson::Value& obj)
	{
		UNREFERENCED_PARAMETER(obj);
		return false;
	}

	/*!*************************************************************************
	Serialize camera component
	****************************************************************************/
	bool CameraComponent::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) const
	{
		UNREFERENCED_PARAMETER(writer);
		return false;
	}
}