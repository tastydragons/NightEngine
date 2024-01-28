#pragma once

#include "NightEngine/Renderer/Texture.h"
#include "NightEngine/Renderer/Shader.h"
#include "NightEngine/Renderer/TextRendering.h"
#include "NightEngine/Audio/AudioEngine.h"

namespace Night
{
	class ResourceManager
	{
	public:
		static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
		static std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		static std::unordered_map<std::string, std::shared_ptr<Font>> fonts;
		static std::unordered_map<std::string, std::shared_ptr<CAudioEngine>> audios;
		static std::unordered_map<std::string, std::shared_ptr<Texture>> Icons;
	public:
		//load texture from file
		static std::shared_ptr<Texture> LoadTexture(std::string name, const std::string& filepath);
		static std::shared_ptr<Texture> GetTexture(std::string name);
		static std::shared_ptr<Texture> GenTexture(unsigned int width, unsigned int height);

		// load Icon texture from file to differentiate the sprite, background and icons texture
		static std::shared_ptr<Texture> LoadIcons(std::string name, const std::string& filepath);
		static std::shared_ptr<Texture> GetIcon(std::string name);

		//load shader from file
		static std::shared_ptr<Shader> LoadShader(std::string name, const std::string& filepath);
		static std::shared_ptr<Shader> GetShader(std::string name);

		static std::shared_ptr<CAudioEngine> GetAudio(std::string name);
		static std::shared_ptr<CAudioEngine> LoadAudio(std::string name, const std::string& filepath);

		//load different type of fonts from file tff
		static void clear();
	private:

	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager() {}
		static std::shared_ptr<CAudioEngine> LoadAudioFromFile(const std::string& filepath);

		//loads texture
		static std::shared_ptr<Texture> LoadTextureFromFile(const std::string& filepath);

		// generates shader from file
		static std::shared_ptr<Shader> LoadShaderFromFile(const std::string& filepath);

		// generates font from file
		static std::shared_ptr<Font> LoadFontFromFile(const std::string& filepath, unsigned int fontsize);

		//loads Icons texture
		static std::shared_ptr<Texture> LoadIconsFromFile(const std::string& filepath);
	};
}