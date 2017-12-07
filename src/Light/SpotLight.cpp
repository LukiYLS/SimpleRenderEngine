#include "SpotLight.h"
#include <sstream>
namespace SRE {

	void SpotLight::upload(Shader::ptr shader)
	{
		std::stringstream ss;
		std::string number;
		ss << _number;
		ss >> number;
		std::string uniform_name_pre = "spotLights[].";
		if (_number < 10)
			uniform_name_pre.insert(11, number, 0, 1);
		else
			uniform_name_pre.insert(11, number, 0, 2);

		shader->use();

		std::string position = uniform_name_pre + "position";
		shader->setVec3(position.c_str(), _uniform.position);

		std::string direction = uniform_name_pre + "direction";
		shader->setVec3(direction.c_str(), _uniform.direction);

		std::string color = uniform_name_pre + "color";
		shader->setVec3(color.c_str(), _uniform.color);

		std::string distance = uniform_name_pre + "distance";
		shader->setFloat(distance.c_str(), _uniform.distance);

		std::string decay = uniform_name_pre + "decay";
		shader->setFloat(decay.c_str(), _uniform.decay);

		std::string coneCos = uniform_name_pre + "coneCos";
		shader->setFloat(coneCos.c_str(), _uniform.coneCos);

		std::string penumbraCos = uniform_name_pre + "penumbraCos";
		shader->setFloat(penumbraCos.c_str(), _uniform.penumbraCos);

		std::string shadow = uniform_name_pre + "shadow";
		shader->setBool(shadow.c_str(), _uniform.shadow);

		std::string shadowBias = uniform_name_pre + "shadowBias";
		shader->setFloat(shadowBias.c_str(), _uniform.shadowBias);

		std::string shadowRadius = uniform_name_pre + "shadowRadius";
		shader->setFloat(shadowRadius.c_str(), _uniform.shadowRadius);

		std::string shadowMapSize = uniform_name_pre + "shadowMapSize";
		shader->setVec2(shadowMapSize.c_str(), _uniform.shadowMapSize);

		shader->unUse();
	}
}