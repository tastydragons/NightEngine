#include "hzpch.h"

#include "JSONSerialization.h"

namespace Night
{
	/*!*************************************************************************
	Json Serializer for calling Derived class Serialize function
	****************************************************************************/
	std::string JSONSerializer::Serialize() const
	{
		rapidjson::StringBuffer ss;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(ss);
		if (Serialize(&writer))
		{
			return ss.GetString();
		}
		return "";
	}

	/*!*************************************************************************
	Json Deserializer for calling Derived class Deserialize function
	****************************************************************************/
	bool JSONSerializer::Deserialize(const std::string& stringbuffer)
	{
		rapidjson::Document doc;
		doc.Parse(stringbuffer.c_str());
		if (!InitDocument(stringbuffer, doc))
		{
			return false;
		}
		else
		{
			Deserialize(doc);
		}
		return true;
	}

	/*!*************************************************************************
	Funtion for Deserializing to JSON file
	****************************************************************************/
	bool JSONSerializer::DeserializeFromFile(const std::string& filePath)
	{
		std::ifstream ifs(filePath, std::ios::in);
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		ifs.close();

		return Deserialize(buffer.str());
	}

	/*!*************************************************************************
	Funtion for Serializing to JSON file
	****************************************************************************/
	bool JSONSerializer::SerializeToFile(const std::string& filePath)
	{
		std::ofstream ofs(filePath);
		std::string str = Serialize();
		ofs << str;
		ofs.flush();
		ofs.close();

		return true;
	}

	/*!*************************************************************************
	Checks if Document can be Read
	****************************************************************************/
	bool JSONSerializer::InitDocument(const std::string& stringbuffer, rapidjson::Document& doc)
	{
		if (stringbuffer.empty())
			return false;

		std::string validJson(stringbuffer);

		return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;
	}
}