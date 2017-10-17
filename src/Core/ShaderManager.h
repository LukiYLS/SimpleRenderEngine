#pragma once
#include "Shader.h"
#include <string>
#include <map>
using namespace std;
namespace Core {

	class ShaderManager {
	protected:
		static ShaderManager* instance;
	public:
		static ShaderManager* getSingleton();
	public:
		Shader::ptr createShader(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
		void remove(const string& name);
		void removeAll();
		Shader::ptr getByName(const string& name);
		void add(const string& name, Shader* shader);
	private:
		void checkCompileErrors(GLuint shader, const char* type);
		typedef map<string, Shader::ptr> ShaderMap;
		ShaderMap _shader_map;
	};
}