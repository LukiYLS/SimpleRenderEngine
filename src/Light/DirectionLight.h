#pragma once

#include "Light.h"
#include "..\Core\OrthographicCamera.h"

namespace SRE {

	struct DirectionalLightUniform {
		Vector3D direction;
		Vector3D color;

		int shadow;
		float shadowBias;
		float shadowRadius;
		Vector2D shadowMapSize;
	};

	class DirectionLight : public Light {

	public:
		typedef std::shared_ptr<DirectionLight> ptr;	

		Vector3D getDirection()const { return _direction; }
		void setDirection(const Vector3D& direction) { _direction = direction; }
		void setUniform(DirectionalLightUniform uniform) { _uniform = uniform; }

		virtual DirectionLight* asDirectionLight() { return this; }
		virtual void upload(Shader::ptr shader);
		virtual LightType getType()const { return DirectionLightType; }

	protected:
		Vector3D _direction;
		float _distance;
		OrthographicCamera::ptr _shadow_camera;
		DirectionalLightUniform _uniform;
	};
}