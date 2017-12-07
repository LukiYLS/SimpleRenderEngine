#pragma once
#include "Light.h"
#include "..\Core\PerspectiveCamera.h"

namespace SRE {

	struct PointLightUniform {
		Vector3D position;
		Vector3D color;
		float distance;
		float decay;

		bool shadow;
		float shadowBias;
		float shadowRadius;
		Vector2D shadowMapSize;
	};
	class PointLight : public Light {

	public:
		typedef std::shared_ptr<PointLight> ptr;
		virtual PointLight* asPointLight() { return this; }
		virtual LightType getType()const { return PointLightType; }
		virtual void upload(Shader::ptr shader);
		void setUniform(PointLightUniform uniform) { _uniform = uniform; }
		float getDistance()const { return _distance; }
		void setDistance(float distance) { _distance = distance; }
		float getDecay()const { return _decay; }
		void setDecay(float decay) { _decay = decay; }
	protected:

		float _distance,_decay;
		PerspectiveCamera::ptr _shadow_camera;
		PointLightUniform _uniform;
	};
}