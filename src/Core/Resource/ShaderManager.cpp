#include "ShaderManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
namespace Core {

	ShaderManager* ShaderManager::instance(0);
	ShaderManager* ShaderManager::getSingleton() {
		if (!instance)
			instance = new ShaderManager();
		return instance;
	}

	Shader::ptr ShaderManager::createShader(const char* name,const char* vertexPath, const char* fragmentPath, const char* geometryPath)
	{
		
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{

			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char * gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

		uint32_t shader_ID = glCreateProgram();
		Shader::ptr shader = make_shared<Shader>(shader_ID);
		glAttachShader(shader_ID, vertex);
		glAttachShader(shader_ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(shader_ID, geometry);
		glLinkProgram(shader_ID);
		checkCompileErrors(shader_ID, "PROGRAM");
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);

		_shader_map.insert(make_pair(name, shader));
		return shader;
	}

	Shader::ptr ShaderManager::getByName(const string& name)
	{
		auto it = _shader_map.find(name);
		if (it != _shader_map.end())
			return it->second;
		return NULL;
	}
	void ShaderManager::remove(const string& name)
	{
		auto it = _shader_map.find(name);
		if (it != _shader_map.end())
			_shader_map.erase(name);
	}
	void ShaderManager::removeAll()
	{
		_shader_map.clear();
	}
	void ShaderManager::checkCompileErrors(GLuint shader, const char* type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
}