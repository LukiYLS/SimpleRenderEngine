#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace Core {


	Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
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
		
		shader_ID = glCreateProgram();
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
	}

	void Shader::use()
	{
		glUseProgram(shader_ID);
	}
	void Shader::setBool(const char* name, bool value)const
	{
		glUniform1i(glGetUniformLocation(shader_ID, name), (int)value);
	}
	void Shader::setInt(const char* name, int value)const
	{
		glUniform1i(glGetUniformLocation(shader_ID, name), value);
	}
	void Shader::setFloat(const char* name, float value)const
	{
		glUniform1f(glGetUniformLocation(shader_ID, name), value);
	}
	void Shader::setVec2(const char* name, const Vector2D& value)const
	{
		glUniform2fv(glGetUniformLocation(shader_ID, name), 1, (GLfloat*)value.ptr());
	}
	void Shader::setVec3(const char* name, const Vector3D& value)const
	{		
		glUniform3fv(glGetUniformLocation(shader_ID, name), 1, (GLfloat*)value.ptr());
	}
	void Shader::setVec4(const char* name, const Vector4D& value)const
	{
		glUniform4fv(glGetUniformLocation(shader_ID, name), 1, (GLfloat*)value.ptr());
	}	
	void Shader::setMat3(const char* name, const Matrix3D& value)const
	{
		glUniformMatrix3fv(glGetUniformLocation(shader_ID, name), 1, GL_FALSE, (GLfloat*)value.getMatrix());
	}
	void Shader::setMat4(const char* name, const Matrix4D& value)const
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_ID, name), 1, GL_FALSE, (GLfloat*)value.getMatrix());
	}
	void Shader::checkCompileErrors(GLuint shader, const char* type)
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