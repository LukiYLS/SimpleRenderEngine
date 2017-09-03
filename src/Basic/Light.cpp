#include "Light.h"

namespace Basic
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
		_shiness = 30;
		_strength = 10;
		_constantAttenuation = 1.0f;
		_linearAttenuation = 0.0f;
		_quatricAttenuaion = 0.0f;

		_spotExponet = 0.0f;
		_spotCosCutoff = 180.0f;
	}
}
