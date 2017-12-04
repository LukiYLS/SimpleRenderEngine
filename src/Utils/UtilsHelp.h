#pragma once
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>

namespace SRE {

	std::string readFileToStr(const char* fileName)
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