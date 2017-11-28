#pragma once


#include "Light.h"
#include "..\Core\PerspectiveCamera.h"

namespace SRE {

	class SpotLight : public Light {

	public:


		typedef std::shared_ptr<SpotLight> ptr;


		virtual LightType getType()const { return SpotLightType; }
	protected:

		float _distance;
		Vector3D _direction;
		float _angle;
		PerspectiveCamera::ptr _shadow_camera;
	};
}
