
#include "UtilsHelp.h"

namespace SRE
{

	std::string StringHelp::Int2String(unsigned int a)
	{
		std::stringstream ss;
		std::string number;
		ss << a;
		ss >> number;
		return number;
	}
	std::string StringHelp::readFileToStr(const char* fileName)
	{
		std::string result;
		std::ifstream shaderFile;
		shaderFile.open(fileName);

		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();


		shaderFile.close();

		return result;
	}
}

