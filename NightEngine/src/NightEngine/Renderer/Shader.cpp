/*!***********************************************************************
 * @file Shader.cpp
 * @author Aloysius Liong Yu Xuan
 * @par DP email: a.liong@digipen.edu
 * @par Course: CSD2401F23-B
 * @par Milestone 1
 * @date 27-9-2023
 * @brief Implementation of the Shader class for managing OpenGL shaders.
 *
 * This file contains the implementation of the Shader class, which is used
 * for loading, compiling, and managing OpenGL shader programs. It provides
 * functions for reading shader source code from files, processing shader
 * source strings, and setting uniform variables in the shader. Additionally,
 * it defines the ShaderLibrary class for managing a collection of shaders.
*************************************************************************/

#include "Shader.h"
#include "hzpch.h"


namespace Night {
	/*!***********************************************************************
	\brief
	Convert a shader type string to its OpenGL enumeration.

	This static function converts a shader type string, such as "vertex" or
	"fragment," into its corresponding OpenGL enumeration (GLenum). This is
	useful when loading and working with shaders in OpenGL.

	\param[in] type
	A string representing the shader type.

	\return
	The OpenGL enumeration representing the shader type
	*************************************************************************/
	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}

		if (type == "fragment") {
			return GL_FRAGMENT_SHADER;
		}

		return 0;
	}

	/*!***********************************************************************
	\brief
	Initialize a shader object by loading and compiling a shader file.

	\param[in] filepath
	The file path to the shader source file.
	*************************************************************************/
	Shader::Shader(const std::string& filepath) {
		std::string source = ReadFile(filepath);
		auto shaderSource = Process(source);
		Compile(shaderSource);

		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	/*!***********************************************************************
	\brief
	Initialize a shader object by specifying separate vertex and fragment shader files.

	\param[in] vertexFile
	The file path to the vertex shader source file.

	\param[in] fragmentFile
	The file path to the fragment shader source file.
	*************************************************************************/
	Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexFile;
		sources[GL_FRAGMENT_SHADER] = fragmentFile;

		Compile(sources);
	}

	/*!***********************************************************************
	\brief
	Read the contents of a file and return them as a string.

	\param[in] filepath
	The file path to the shader source file.

	\return
	A string containing the contents of the file, or an empty string if the file could not be read.
	*************************************************************************/
	std::string Shader::ReadFile(const std::string& filepath) {
		std::ifstream ifs;

		ifs.open(filepath);
		std::string result;
		if (ifs) {
			ifs.seekg(0, std::ios::end);
			result.resize(ifs.tellg());

			ifs.seekg(0, std::ios::beg);
			ifs.read(&result[0], result.size());
			ifs.close();
		}

		else {
			NIGHT_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	/*!***********************************************************************
	\brief
	Process a shader source string to separate it into shader types.

	\param[in] src
	The shader source code as a single string.

	\return
	An unordered map containing shader sources separated by their corresponding shader types.
	*************************************************************************/
	std::unordered_map<GLenum, std::string> Shader::Process(const std::string& src) {
		std::unordered_map<GLenum, std::string> shaderSrcs;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = src.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = src.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string type = src.substr(begin, eol - begin);

			size_t nextLinePos = src.find_first_not_of("\r\n", eol);
			pos = src.find(typeToken, nextLinePos);
			shaderSrcs[ShaderTypeFromString(type)] = src.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? src.size() - 1 : nextLinePos));

		}
		return shaderSrcs;
	}

	/*!***********************************************************************
	\brief
	Compile and link shader program from separate shader sources.

	\param[in] shaderSrcs
	An unordered map containing shader sources separated by their corresponding shader types.
	*************************************************************************/
	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs) {
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> glShaderID;
		int glShaderId = 0;

		for (auto& kv : shaderSrcs) {
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				break;
			}

			glAttachShader(program, shader);
			glShaderID[glShaderId++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (auto id : glShaderID) {
				glDeleteShader(id);
			}
			return;
		}

		for (auto id : glShaderID) {
			glDetachShader(program, id);
		}

		m_rendererID = program;
	}

	/*!***********************************************************************
	\brief
	Destructor for the Shader class.

	This destructor is responsible for cleaning up the resources associated
	with the shader program, such as deleting the OpenGL program object.
	*************************************************************************/
	Shader::~Shader() {
		glDeleteProgram(m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Bind the shader for use in rendering.

	This function binds the shader for use in rendering by invoking the
	`glUseProgram` function with the shader program's OpenGL identifier.
	*************************************************************************/
	void Shader::Bind() const {
		glUseProgram(m_rendererID);
	}

	/*!***********************************************************************
	\brief
	Unbind the currently bound shader.

	This function unbinds the currently bound shader by setting the active
	shader program to 0 using `glUseProgram(0)`. This is typically done to
	clear the shader state before rendering with a different shader.
	*************************************************************************/
	void Shader::Unbind() const {
		glUseProgram(0);
	}

	/*!***********************************************************************
	\brief
	Set a boolean uniform in the shader.

	This function sets the value of a boolean uniform variable in the shader.
	You need to provide the uniform variable's name and the boolean value
	you want to assign to it.

	\param[in] name
	The name of the boolean uniform variable.

	\param[in] val
	The boolean value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLboolean val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform1i(loc, val);
	}

	/*!***********************************************************************
	\brief
	Set an integer uniform in the shader.

	This function sets the value of an integer uniform variable in the shader.
	You need to provide the uniform variable's name and the integer value
	you want to assign to it.

	\param[in] name
	The name of the integer uniform variable.

	\param[in] val
	The integer value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLint val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform1i(loc, val);
	}

	/*!***********************************************************************
	\brief
	Set a float uniform in the shader.

	This function sets the value of a floating-point uniform variable in the shader.
	You need to provide the uniform variable's name and the floating-point value
	you want to assign to it.

	\param[in] name
	The name of the float uniform variable.

	\param[in] val
	The float value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLfloat val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform1f(loc, val);
	}

	/*!***********************************************************************
	\brief
	Set a 2D vector uniform in the shader.

	This function sets the value of a 2D vector (two floating-point values)
	uniform variable in the shader. You need to provide the uniform variable's
	name and the two float values (x and y components) to assign to it.

	\param[in] name
	The name of the 2D vector uniform variable.

	\param[in] x
	The value to assign to the x-component of the vector.

	\param[in] y
	The value to assign to the y-component of the vector.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLfloat x, GLfloat y) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform2f(loc, x, y);
	}

	/*!***********************************************************************
	\brief
	Set a 3D vector uniform in the shader.

	This function sets the value of a 3D vector (three floating-point values)
	uniform variable in the shader. You need to provide the uniform variable's
	name and the three float values (x, y, and z components) to assign to it.

	\param[in] name
	The name of the 3D vector uniform variable.

	\param[in] x
	The value to assign to the x-component of the vector.

	\param[in] y
	The value to assign to the y-component of the vector.

	\param[in] z
	The value to assign to the z-component of the vector.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform3f(loc, x, y, z);
	}

	/*!***********************************************************************
	\brief
	Set a 4D vector uniform in the shader.

	This function sets the value of a 4D vector (four floating-point values)
	uniform variable in the shader. You need to provide the uniform variable's
	name and the four float values (x, y, z, and w components) to assign to it.

	\param[in] name
	The name of the 4D vector uniform variable.

	\param[in] x
	The value to assign to the x-component of the vector.

	\param[in] y
	The value to assign to the y-component of the vector.

	\param[in] z
	The value to assign to the z-component of the vector.

	\param[in] w
	The value to assign to the w-component of the vector.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform4f(loc, x, y, z, w);
	}

	/*!***********************************************************************
	\brief
	Set a 2D vector uniform in the shader using glm::vec2.

	This function sets the value of a 2D vector (two floating-point values)
	uniform variable in the shader using a `glm::vec2`. You need to provide
	the uniform variable's name and the `glm::vec2` value to assign to it.

	\param[in] name
	The name of the 2D vector uniform variable.

	\param[in] val
	The `glm::vec2` value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, glm::vec2 const& val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform2f(loc, val.x, val.y);
	}

	/*!***********************************************************************
	\brief
	Set a 3D vector uniform in the shader using glm::vec3.

	This function sets the value of a 3D vector (three floating-point values)
	uniform variable in the shader using a `glm::vec3`. You need to provide
	the uniform variable's name and the `glm::vec3` value to assign to it.

	\param[in] name
	The name of the 3D vector uniform variable.

	\param[in] val
	The `glm::vec3` value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, glm::vec3 const& val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform3f(loc, val.x, val.y, val.z);
	}

	/*!***********************************************************************
	\brief
	Set a 4D vector uniform in the shader using glm::vec4.

	This function sets the value of a 4D vector (four floating-point values)
	uniform variable in the shader using a `glm::vec4`. You need to provide
	the uniform variable's name and the `glm::vec4` value to assign to it.

	\param[in] name
	The name of the 4D vector uniform variable.

	\param[in] val
	The `glm::vec4` value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, glm::vec4 const& val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniform4f(loc, val.x, val.y, val.z, val.w);
	}

	/*!***********************************************************************
	\brief
	Set a 3x3 matrix uniform in the shader using glm::mat3.

	This function sets the value of a 3x3 matrix uniform variable in the shader
	using a `glm::mat3`. You need to provide the uniform variable's name and
	the `glm::mat3` value to assign to it.

	\param[in] name
	The name of the 3x3 matrix uniform variable.

	\param[in] val
	The `glm::mat3` value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, glm::mat3 const& val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniformMatrix3fv(loc, 1, GL_FALSE, &val[0][0]);
	}

	/*!***********************************************************************
	\brief
	Set a 4x4 matrix uniform in the shader using glm::mat4.

	This function sets the value of a 4x4 matrix uniform variable in the shader
	using a `glm::mat4`. You need to provide the uniform variable's name and
	the `glm::mat4` value to assign to it.

	\param[in] name
	The name of the 4x4 matrix uniform variable.

	\param[in] val
	The `glm::mat4` value to assign to the uniform.
	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, glm::mat4 const& val) {
		GLint loc = glGetUniformLocation(m_rendererID, name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &val[0][0]);
	}

	/*!***********************************************************************
	\brief

	*************************************************************************/
	void Shader::SetUniform(GLchar const* name, GLint* val, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_rendererID, name);
		glUniform1iv(location, count, val);
	}

	/*!***********************************************************************
	\brief
	Add a shader to the shader library.

	This function adds a shader to the shader library using its name as
	the key for retrieval.

	\param[in] shader
	A reference to the shader to be added to the library.
	*************************************************************************/
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	/*!***********************************************************************
	\brief
	Add a shader to the shader library with a custom name.

	This function adds a shader to the shader library with a custom name
	as the key for retrieval.

	\param[in] name
	The custom name to associate with the shader.

	\param[in] shader
	A reference to the shader to be added to the library.
	*************************************************************************/
	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		m_Shaders[name] = shader;
	}

	/*!***********************************************************************
	\brief
	Retrieve a shader from the shader library by name.

	This function retrieves a shader from the shader library using its name
	as the key for retrieval.

	\param[in] name
	The name of the shader to retrieve.

	\return
	A reference to the shader if found; otherwise, an empty reference.
	*************************************************************************/
	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		return m_Shaders[name];
	}

	/*!***********************************************************************
	\brief
	Load and add a shader to the shader library with a custom name.

	This function loads a shader from a file, adds it to the shader library
	with a custom name, and returns a reference to the loaded shader.

	\param[in] name
	The custom name to associate with the loaded shader.

	\param[in] filepath
	The file path to the shader source file.

	\return
	A reference to the loaded shader.
	*************************************************************************/
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = std::make_shared<Shader>(filepath);
		Add(name, shader);
		return shader;
	}

	/*!***********************************************************************
	\brief
	Load and add a shader to the shader library.

	This function loads a shader from a file, adds it to the shader library,
	and returns a reference to the loaded shader.

	\param[in] filepath
	The file path to the shader source file.

	\return
	A reference to the loaded shader.
	*************************************************************************/
	Ref<Shader> ShaderLibrary::Load(const std::string& filepath) {
		auto shader = std::make_shared<Shader>(filepath);
		Add(shader);
		return shader;
	}
}