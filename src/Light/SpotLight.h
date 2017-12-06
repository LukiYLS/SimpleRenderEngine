#pragma once


#include "Light.h"
#include "..\Core\PerspectiveCamera.h"

namespace SRE {


	struct SpotLightUniform {
		Vector3D position;
		Vector3D direction;
		Vector3D color;
		float distance;
		float decay;
		float coneCos;
		float penumbraCos;

		int shadow;
		float shadowBias;
		float shadowRadius;
		Vector2D shadowMapSize;
	};
	class SpotLight : public Light {

	public:
		typedef std::shared_ptr<SpotLight> ptr;
		virtual SpotLight* asSpotLight() { return this; }
		virtual void upload(Shader::ptr shader);
		virtual LightType getType()const { return SpotLightType; }
		void setUniform(SpotLightUniform uniform) { _uniform = uniform; }		
	protected:

		float _distance;
		Vector3D _direction;
		float _angle;
		PerspectiveCamera::ptr _shadow_camera;
		SpotLightUniform _uniform;
	};
}
