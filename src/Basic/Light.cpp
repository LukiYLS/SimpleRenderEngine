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
		_ambientIntensity = 0.1f;
		_diffuseIntensity = 0.1f;		
		_lightPosition = glm::vec3(0.0f, 0.0f, -1.0f);
		_lightDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		_shiness = 80;
		_strength = 10;
		_constantAttenuation = 1.0f;
		_linearAttenuation = 0.0f;
		_quatricAttenuaion = 0.0f;
		_spotExponet = 0.0f;
		_spotCosCutoff = 0.3;
		has_Shadow = false;
	}
	bool Light::enableShadow()
	{
		has_Shadow = true;
	}
}
