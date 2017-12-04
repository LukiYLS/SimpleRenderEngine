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
		_shaderFileMap.insert(std::pair<std::string, const char*>("attribute_vertex", ""));
	}

	void ShaderLib::init()
	{
		std::string path_vertex = "../../../src/ShaderLib/vertex/";
		std::string path_frag = "../../../src/ShaderLib/fragment/";

		_shaderLibMap.insert(std::pair<std::string, std::string>("attribute_vertex", readFileToStr((path_vertex + "attribute_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("begin_vertex", readFileToStr((path_vertex + "begin_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_pars_vertex", readFileToStr((path_vertex + "clipping_planes_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_vertex", readFileToStr((path_vertex + "clipping_planes_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_pars_vertex", readFileToStr((path_vertex + "color_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_vertex", readFileToStr((path_vertex + "color_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("defaultnormal_vertex", readFileToStr((path_vertex + "defaultnormal_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("envmap_pars_vertex", readFileToStr((path_vertex + "envmap_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("envmap_vertex", readFileToStr((path_vertex + "envmap_vertex.glsl").c_str())));

		_shaderLibMap.insert(std::pair<std::string, std::string>("fog_pars_vertex", readFileToStr((path_vertex + "fog_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("fog_vertex", readFileToStr((path_vertex + "fog_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("logdepthbuf_vertex", readFileToStr((path_vertex + "logdepthbuf_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("logdepthbuf_pars_vertex", readFileToStr((path_vertex + "logdepthbuf_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("morphnormal_vertex", readFileToStr((path_vertex + "morphnormal_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("morphtarget_pars_vertex", readFileToStr((path_vertex + "morphtarget_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("morphtarget_vertex", readFileToStr((path_vertex + "morphtarget_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("project_vertex", readFileToStr((path_vertex + "project_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("shadowmap_vertex", readFileToStr((path_vertex + "shadowmap_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("shadowmask_pars_fragment", readFileToStr((path_vertex + "shadowmask_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("skinning_pars_vertex", readFileToStr((path_vertex + "skinning_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("skinning_vertex", readFileToStr((path_vertex + "skinning_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("skinnormal_vertex", readFileToStr((path_vertex + "skinnormal_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("uv_vertex", readFileToStr((path_vertex + "uv_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("uv_pars_vertex", readFileToStr((path_vertex + "uv_pars_vertex.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("worldpos_vertex", readFileToStr((path_vertex + "worldpos_vertex.glsl").c_str())));


		_shaderLibMap.insert(std::pair<std::string, std::string>("alphamap_fragment", readFileToStr((path_vertex + "alphamap_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("alphamap_pars_fragment", readFileToStr((path_vertex + "alphamap_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("alphatest_fragment", readFileToStr((path_vertex + "alphatest_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("aomap_fragment", readFileToStr((path_vertex + "aomap_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("aomap_pars_fragment", readFileToStr((path_vertex + "aomap_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("bsdfs", readFileToStr((path_vertex + "bsdfs.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_fragment", readFileToStr((path_vertex + "clipping_planes_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("clipping_planes_pars_fragment", readFileToStr((path_vertex + "clipping_planes_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_fragment", readFileToStr((path_vertex + "color_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("color_pars_fragment", readFileToStr((path_vertex + "color_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("envmap_fragment", readFileToStr((path_vertex + "envmap_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("envmap_pars_fragment", readFileToStr((path_vertex + "envmap_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("fog_fragment", readFileToStr((path_vertex + "fog_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("fog_pars_fragment", readFileToStr((path_vertex + "fog_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lightmap_fragment", readFileToStr((path_vertex + "lightmap_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lightmap_pars_fragment", readFileToStr((path_vertex + "lightmap_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_pars", readFileToStr((path_vertex + "lights_pars.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_phong_fragment", readFileToStr((path_vertex + "lights_phong_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_phong_pars_fragment", readFileToStr((path_vertex + "lights_phong_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_physical_fragment", readFileToStr((path_vertex + "lights_physical_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_physical_pars_fragment", readFileToStr((path_vertex + "lights_physical_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("lights_template", readFileToStr((path_vertex + "lights_template.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("logdepthbuf_fragment", readFileToStr((path_vertex + "logdepthbuf_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("logdepthbuf_pars_fragment", readFileToStr((path_vertex + "logdepthbuf_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("map_fragment", readFileToStr((path_vertex + "map_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("map_pars_fragment", readFileToStr((path_vertex + "map_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("normal_flip", readFileToStr((path_vertex + "normal_flip.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("normal_fragment", readFileToStr((path_vertex + "normal_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("specularmap_fragment", readFileToStr((path_vertex + "specularmap_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("specularmap_pars_fragment", readFileToStr((path_vertex + "specularmap_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("tonemapping_fragment", readFileToStr((path_vertex + "tonemapping_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("tonemapping_pars_fragment", readFileToStr((path_vertex + "tonemapping_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("tonemapping_pars_fragment", readFileToStr((path_vertex + "tonemapping_pars_fragment.glsl").c_str())));
		_shaderLibMap.insert(std::pair<std::string, std::string>("uv_pars_fragment", readFileToStr((path_vertex + "uv2_pars_fragment.glsl").c_str())));



	}

	std::string ShaderLib::assembleBasicVertexShader()
	{

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