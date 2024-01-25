/*!***********************************************************************
 * @file Shader.h
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Declarations of the Shader and ShaderLibrary classes for OpenGL shader management.
 *
 * This file contains the declarations of the Shader and ShaderLibrary classes,
 * which are used for loading, compiling, managing OpenGL shader programs, and
 * managing a collection of shaders, respectively. The Shader class provides
 * functions for setting uniform variables in shaders and processing shader source code.
*************************************************************************/

#pragma once

#include "../Engine/Core.h"
#include <string>
#include <glm.hpp>
#include <unordered_map>
#include <GL/glew.h>

namespace Night {
	class Shader {
	public:
		Shader(const std::string& filepath);
		Shader(const std::string& vertexFile, const std::string& fragmentFile);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(GLchar const* name, GLboolean val);
		void SetUniform(GLchar const* name, GLint val);
		void SetUniform(GLchar const* name, GLfloat val);
		void SetUniform(GLchar const* name, GLfloat x, GLfloat y);
		void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
		void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		void SetUniform(GLchar const* name, glm::vec2 const& val);
		void SetUniform(GLchar const* name, glm::vec3 const& val);
		void SetUniform(GLchar const* name, glm::vec4 const& val);
		void SetUniform(GLchar const* name, glm::mat3 const& val);
		void SetUniform(GLchar const* name, glm::mat4 const& val);
		void SetUniform(GLchar const* name, GLint* value, uint32_t count);

		GLuint ReturnShaderID() {return m_rendererID;}

		const std::string& GetName() const { return m_Name; }

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> Process(const std::string& src);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs);

	private:
		GLuint m_rendererID;
		std::string m_Name;
	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);

		Ref<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}