#include "Light.h"
#include "../Math/MathHelper.h"
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
		_ambient = Vector3D(0.05f, 0.05f, 0.05f);
		_diffuse = Vector3D(0.8f, 0.8f, 0.8f);
		_specular = Vector3D(0.05f, 0.05f, 0.05f);
		_position = Vector3D(0.0f, 0.0f, 1.0f);
		_direction = Vector3D(0.0f, 0.0f, -1.0f);
		_innerCutOff = MathHelper::radian(10.0f);
		_outerCutOff = MathHelper::radian(30.0f);
		_constant = 1.0f;
		_linear = 0.0f;
		_quadratic = 0.0f;
		_isShadow = false;
	}

	
}
