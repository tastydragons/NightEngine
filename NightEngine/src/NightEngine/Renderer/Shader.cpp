#include "nepch.h"

#include "NightEngine/Renderer/Shader.h"

namespace Night
{
	/*!*************************************************************************
	Read the shader from file .shader file and determine vertex and fragment part
	****************************************************************************/
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment")
			return GL_FRAGMENT_SHADER;

		NIGHT_CORE_ASSERT(false, "Wrong Shader Type!");
		return 0;
	}
	/*!*************************************************************************
	How the shader is been read and set
	****************************************************************************/
	Shader::Shader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shadersources = Processing(source);
		Compile(shadersources);


		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		mName = filepath.substr(lastSlash, count);
	}
	/*!*************************************************************************
	Set the correct vertex and fragment source into the compile function
	****************************************************************************/
	Shader::Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:mName(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}
	/*!*************************************************************************
	Delete the shader program
	****************************************************************************/
	Shader::~Shader()
	{
		glDeleteProgram(mRendererID);
	}
	/*!*************************************************************************
	Set the correct file path to read the shader
	****************************************************************************/
	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string out;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				out.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&out[0], size);
				in.close();
			}
			else
			{
				NIGHT_CORE_ERROR("Cannot read from file '{0}'", filepath);
			}
		}
		else
		{
			NIGHT_CORE_ERROR("Cannot open file '{0}'", filepath);
		}
		return out;
	}
	/*!*************************************************************************
	Set how the shader is being read
	****************************************************************************/
	std::unordered_map<GLenum, std::string> Shader::Processing(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shader;

		const char* Token = "#shader";
		size_t TokenLength = strlen(Token);
		size_t pos = source.find(Token, 0); //start from shader declaration

		while (pos != std::string::npos)
		{
			size_t endofline = source.find_first_of("\r\n", pos);//end of (#shader line)
			size_t begin = pos + TokenLength + 1; //start of shader type name(after #shader)

			std::string type = source.substr(begin, endofline - begin);

			size_t nextLine = source.find_first_not_of("\r\n", endofline);//start of shader code after vertex/fragment declaration

			pos = source.find(Token, nextLine); //start of next shader type declaration line

			shader[ShaderTypeFromString(type)] = (pos == std::string::npos) ?
				source.substr(nextLine) :
				source.substr(nextLine, pos - nextLine);

		}
		return shader;
	}
	/*!*************************************************************************
	Compile the shader
	****************************************************************************/
	void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::array<GLenum, 2> shaderIDs; //only have 2 shaders for now
		int shaderIDIndex = 0;
		for (auto& i : shaderSources)
		{
			GLenum type = i.first;
			const std::string& source = i.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			int Compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &Compiled);
			if (Compiled == GL_FALSE)
			{
				int length = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

				std::vector<GLchar> infolog(length);
				glGetShaderInfoLog(shader, length, &length, &infolog[0]);

				glDeleteShader(shader);

				NIGHT_CORE_ERROR("{0}", infolog.data());
				NIGHT_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			shaderIDs[shaderIDIndex++] = shader;
		}

		mRendererID = program;

		//link program
		glLinkProgram(program);

		int Linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &Linked);
		if (Linked == GL_FALSE)
		{
			int length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

			// length includes the NULL character
			std::vector<GLchar> infolog(length);
			glGetProgramInfoLog(program, length, &length, &infolog[0]);

			// Don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);

			NIGHT_CORE_ERROR("{0}", infolog.data());
			NIGHT_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}
	/*!*************************************************************************
	Bind the shader
	****************************************************************************/
	void Shader::Bind() const
	{
		glUseProgram(mRendererID);
	}
	/*!*************************************************************************
	Unbind the shader
	****************************************************************************/
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	/*!*************************************************************************
	Overload function for the specific type (int)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}
	/*!*************************************************************************
	Overload function for the specific type (int)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, int* value, unsigned int count)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1iv(location, count, value);
	}
	/*!*************************************************************************
	Overload function for the specific type (float)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1f(location, value);
	}
	/*!*************************************************************************
	Overload function for the specific type (float , float)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}
	/*!*************************************************************************
	Overload function for the specific type (3 float)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}
	/*!*************************************************************************
	Overload function for the specific type (4 float)
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}
	/*!*************************************************************************
	Overload function for the specific type matrix 3x3
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, const glm::mat3& value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
	}
	/*!*************************************************************************
	Overload function for the specific type matrix 4 x 4
	****************************************************************************/
	void Shader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
	}
	void Shader::SetUniform(const std::string& name, bool value)
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());
		glUniform1i(location, value);
	}
}