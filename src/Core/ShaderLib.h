#pragma once
#include <string>
#include <map>
namespace SRE {

	class ShaderLib {

	public:
		static ShaderLib* getSingleton();
		ShaderLib();
		void init();
		//std::string getShaderStr(const std::string& key);
		//std::string readFileToStr(const char* fileName);
	protected:

		static ShaderLib* _inst;


		std::map<std::string, std::string>_shaderLibMap;

		std::map<std::string, const char*> _shaderFileMap;
	};
}