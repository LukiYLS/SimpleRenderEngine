#include "ShaderLib.h"
#include <fstream>
#include <sstream>
#include <algorithm>
namespace SRE {

	void ShaderLib::init()
	{
		_shaderLibMap.insert(std::pair<std::string, std::string>("vertex_attribute", readFileToStr("../"));

	}
	std::string ShaderLib::readFileToStr(const char* fileName)
	{
		std::string result;
		std::ifstream shaderFile;
		shaderFile.open(fileName);
		
		std::stringstream shaderStream, fShaderStream;

		shaderStream << shaderFile.rdbuf();
		

		shaderFile.close();

		return result;
	}

}