#include "ShaderLib.h"
#include <fstream>
#include <sstream>
#include <algorithm>
namespace SRE {

	ShaderLib* ShaderLib::_inst(0);

	ShaderLib* ShaderLib::getSingleton()
	{
		if (!_inst)
			_inst = new ShaderLib();
		return _inst;
	}

	ShaderLib::ShaderLib()
	{
		_shaderFileMap.insert(std::pair<std::string, const char*>("attribute_vertex",""));
	}

	void ShaderLib::init()
	{
		std::string path = "../../../src/ShaderLib/vertex/";
		std::string path_frag = "../../../src/ShaderLib/fragment/";

		_shaderLibMap.insert(std::pair<std::string, std::string>("attribute_vertex", readFileToStr((path + "/attribute_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("begin_vertex", readFileToStr((path + "/begin_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_pars_vertex", readFileToStr((path + "/clipping_planes_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_vertex", readFileToStr((path + "/clipping_planes_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_pars_vertex", readFileToStr((path + "/color_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_vertex", readFileToStr((path + "color_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("defaultnormal_vertex", readFileToStr((path + "defaultnormal_vertex.glsl").c_str())));


	}
	std::string ShaderLib::readFileToStr(const char* fileName)
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