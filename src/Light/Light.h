#pragma once
#include "..\Core\Object.h"
#include "..\Core\Shader.h"
#include "..\Core\FrameBuffer.h"
#include "..\Core\Camera.h"
namespace SRE {	

	class DirectionLight;
	class PointLight;
	class SpotLight;
	class HemisphereLight;
	class Light : public  Object{
	public:
		enum LightType {
			AmbientLightType,
			DirectionLightType,
			PointLightType,
			SpotLightType,
			HemisphereLightType,
			NoneType
		};	
		struct ShadowInfo {
			Matrix4D shadowMatrix;
			FrameBuffer::ptr depthFBO;
		};
		Light() :_color(Vector3D(1.0, 1.0, 1.0)), _intensity(1.0), _castShadow(true) {

		}
		virtual ~Light() = default;

	public:
		typedef std::shared_ptr<Light> ptr;
		virtual Light* asLight() { return this; }
		virtual const Light* asLight() const { return this; }
		virtual DirectionLight* asDirectionLight() { return NULL; }
		virtual PointLight* asPointLight() { return NULL; }
		virtual SpotLight* asSpotLight() { return NULL; }
		virtual HemisphereLight* asHemisphereLight() { return NULL; }

		void setColor(const Vector3D& color) { _color = color; }
		Vector3D getColor()const { return _color; }

		void setIntensity(float intensity) { _intensity = intensity; }
		float getIntensity()const { return _intensity; }

		void setCastShadow(bool isCast) { _castShadow = isCast; }
		bool getCastShadow()const { return _castShadow; }

		virtual void setNumber(unsigned int number) { _number = number; }
		virtual void upload(Shader* shader) { /*shader->setVec3("ambientLightColor",);*/ }
		virtual void uploadShadow(Shader* shader, unsigned int& currectTextureUnit) {}

		virtual LightType getType()const { return NoneType; }

		virtual Camera* getShadowCamera()const { return NULL; }
		virtual void setShadowCamera(Camera* camera){}

		Vector2D getShadowMapSize()const { return _shadowMapSize; }
		void setShadowMapSize(Vector2D shadowMapSize) { _shadowMapSize = shadowMapSize; }

		float getShadowBias()const { return _shadowBias; }
		void setShadowBias(float shadowBias) { _shadowBias = shadowBias; }

		float getShadowRadius()const { return _shadowRadius; }
		void setShadowRadius(float shadowRadius) { _shadowRadius = shadowRadius; }

		void setShadowInfo(ShadowInfo shadowInfo) { _shadowInfo = shadowInfo; }
		ShadowInfo getShadowInfo()const { return _shadowInfo; }
	protected:

		Vector3D _color;
		unsigned int _number;
		float _intensity;
		bool _castShadow;
		ShadowInfo _shadowInfo;
		Vector2D _shadowMapSize;
		float _shadowBias, _shadowRadius;
	};
}