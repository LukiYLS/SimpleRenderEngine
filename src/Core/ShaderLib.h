#pragma once
#include <string>
#include <map>
namespace SRE {

	class ShaderLib {

	public:

		ShaderLib();
		void init();
		std::string getShaderStr(const std::string& key);
		std::string readFileToStr(const char* fileName);
	protected:

		static ShaderLib* inst;

		std::map<std::string, std::string>_shaderLibMap;
	};
}