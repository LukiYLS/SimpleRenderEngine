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

		//Shader::ptr createMesh(const string& fileName, Loader::ptr loader, const string& name);
		//Shader::ptr createMesh(Pararmeter::ptr paras, MeshFactory::ptr factory, const string& name);
		Shader::ptr loadFromFile(const string& vs);
		void remove(const string& name);
		void removeAll();
		Shader::ptr getByName(const string& name);
	private:

		typedef map<string, Shader::ptr> ShaderMap;
		ShaderMap _shader_map;
	};
}