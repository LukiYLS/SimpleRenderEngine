#pragma once


#include "Light.h"
#include "..\Core\PerspectiveCamera.h"

namespace SRE {

	class PointLight : public Light {

	public:


		typedef std::shared_ptr<PointLight> ptr;


		virtual LightType getType()const { return PointLightType; }
	protected:

		float _distance;
		PerspectiveCamera::ptr _shadow_camera;
	};
}