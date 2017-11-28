#pragma once

#include "Light.h"
#include "..\Core\OrthographicCamera.h"

namespace SRE {

	class DirectionLight : public Light {

	public:


		typedef std::shared_ptr<DirectionLight> ptr;

		virtual LightType getType()const { return DirectionLightType; }

	protected:


		Vector3D _direction;
		float _distance;
		OrthographicCamera::ptr _shadow_camera;
	};
}