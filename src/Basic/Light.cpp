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
		_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		_ambientIntensity = 0.5f;
		_diffuseIntensity = 0.7f;		
		_lightPosition = glm::vec3(0.0f, 0.0f, -3.0f);
		_lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		_shiness = 50;
		_strength = 1;
		_constantAttenuation = 1.0f;
		_linearAttenuation = 0.0f;
		_quatricAttenuaion = 0.0f;
		_spotExponet = 0.0f;
		_spotCosCutoff = 180.0f;
	}
}
