#include "hzpch.h"

#include "ResourceManager.h"

namespace Night
{
	std::unordered_map <std::string, std::shared_ptr<Texture>> ResourceManager::textures;
	std::unordered_map <std::string, std::shared_ptr<Shader>> ResourceManager::shaders;
	std::unordered_map <std::string, std::shared_ptr<CAudioEngine>> ResourceManager::audios;
	std::unordered_map <std::string, std::shared_ptr<Texture>> ResourceManager::Icons;

	/*!*************************************************************************
	Load texture
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::LoadTexture(std::string name, const std::string& filepath)
	{
		return textures[name] = ResourceManager::LoadTextureFromFile(filepath);
	}

	/*!*************************************************************************
	Retrieve texture based on filename
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::GetTexture(std::string name)
	{
		return textures[name];
	}

	/*!*************************************************************************
	Load texture from filepath
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::LoadTextureFromFile(const std::string& filepath)
	{
		auto m_texture = std::make_shared<Texture>();
		m_texture->GenTexture(filepath);
		return m_texture;
	}

	/*!*************************************************************************
	Generate texture
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::GenTexture(unsigned int width, unsigned int height)
	{
		auto m_texture = std::make_shared<Texture>();
		m_texture->GenTexture(width, height);
		return m_texture;
	}

	/*!*************************************************************************
	Load Icons from file
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::LoadIcons(std::string name, const std::string& filepath)
	{

		return Icons[name] = ResourceManager::LoadIconsFromFile(filepath);
	}

	/*!*************************************************************************
	Retrieve icons based on filename
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::GetIcon(std::string name)
	{
		return Icons[name];
	}

	/*!*************************************************************************
	Load shader based on filepath
	****************************************************************************/
	std::shared_ptr<Shader> ResourceManager::LoadShaderFromFile(const std::string& filepath)
	{
		auto m_shader = std::make_shared<Shader>(filepath);

		return m_shader;
	}

	/*!*************************************************************************
	Retrieve texture based on filename and filepath
	****************************************************************************/
	std::shared_ptr<Shader> ResourceManager::LoadShader(std::string name, const std::string& filepath)
	{
		return shaders[name] = ResourceManager::LoadShaderFromFile(filepath);
	}

	/*!*************************************************************************
	Retrieve shader on filename
	****************************************************************************/
	std::shared_ptr<Shader> ResourceManager::GetShader(std::string name)
	{
		return shaders[name];
	}

	/*!*************************************************************************
	Load font based on filepath
	****************************************************************************/
	std::shared_ptr<Font> ResourceManager::LoadFontFromFile(const std::string& filepath, unsigned int fontsize)
	{
		auto m_font = std::make_shared<Font>();
		m_font->Load(filepath, fontsize);
		return m_font;
	}

	/*!*************************************************************************
	Load icons based on filepath
	****************************************************************************/
	std::shared_ptr<Texture> ResourceManager::LoadIconsFromFile(const std::string& filepath)
	{
		auto mIcons = std::make_shared<Texture>();
		mIcons->GenTexture(filepath);
		return mIcons;
	}

	/*!*************************************************************************
	Load audio based on filepath
	****************************************************************************/
	std::shared_ptr<CAudioEngine> ResourceManager::LoadAudioFromFile(const std::string& filepath)
	{
		auto m_audio = std::make_shared<CAudioEngine>();
		m_audio->Loadsound(filepath);
		return m_audio;
	}

	/*!*************************************************************************
	Retrieve audio based on filename
	****************************************************************************/
	std::shared_ptr<CAudioEngine> ResourceManager::GetAudio(std::string name)
	{
		return audios[name];
	}

	/*!*************************************************************************
	Load audio
	****************************************************************************/
	std::shared_ptr<CAudioEngine> ResourceManager::LoadAudio(std::string name, const std::string& filepath)
	{
		(void)filepath;
		return std::shared_ptr<CAudioEngine>();
	}

	/*!*************************************************************************
	Clear various maps for resources being managed
	****************************************************************************/
	void Night::ResourceManager::clear()
	{
		textures.clear();
		Icons.clear();
		shaders.clear();
	}
}