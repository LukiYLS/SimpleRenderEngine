#include "Light.h"

namespace Core
{
	Light::Light(void)
	{
		init();
	}

	Light::Light(unsigned int lightnum)
	{
		init();
		_lightnum = lightnum;
	}

	Light::~Light(void)
	{
	}

	void Light::init(void)
	{
		_lightnum = 0;
		_ambient = glm::vec3(0.05f, 0.05f, 0.05f);
		_diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
		_specular = glm::vec3(0.05f, 0.05f, 0.05f);
		_position = glm::vec3(0.0f, 0.0f, 1.0f);
		_direction = glm::vec3(0.0f, 0.0f, -1.0f);
		_innerCutOff = glm::radians(10.0f);
		_outerCutOff = glm::radians(30.0f);
		_constant = 1.0f;
		_linear = 0.0f;
		_quadratic = 0.0f;
		_isShadow = false;
	}

	Light::ptr Light::createLight(Parameter::ptr paras)
	{
		LightType type = paras->getValue("type");
		glm::vec3 ambient = paras->getValue("ambient");

	}
}
