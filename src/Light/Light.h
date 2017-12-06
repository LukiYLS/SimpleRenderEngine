#pragma once
#include "..\Core\Object.h"
#include "..\Core\HardwareBuffer\ColorValue.h"
#include "..\Core\Shader.h"
namespace SRE {	


	class DirectionLight;
	class PointLight;
	class SpotLight;
	class HemisphereLight;
	class Light {
	public:

		enum LightType {

			DirectionLightType,
			PointLightType,
			SpotLightType,
			HemisphereLightType,
			NoneType
		};
		
		Light() {};
		virtual ~Light() {};

	public:
		typedef std::shared_ptr<Light> ptr;
		virtual Light* asLight() { return this; }
		virtual const Light* asLight() const { return this; }

		virtual DirectionLight* asDirectionLight() { return NULL; }
		virtual PointLight* asPointLight() { return NULL; }
		virtual SpotLight* asSpotLight() { return NULL; }
		virtual HemisphereLight* asHemisphereLight() { return NULL; }

		void setColor(ColorValue color) { _color = color; }
		ColorValue getColor()const { return _color; }

		void setIntensity(float intensity) { _intensity = intensity; }
		float getIntensity()const { return _intensity; }

		void setCastShadow(bool isCast) { _castShadow = isCast; }
		bool getCastShadow()const { return _castShadow; }

		virtual void setNumber(unsigned int number) { _number = number; }
		virtual void upload(Shader::ptr shader) {}
		virtual LightType getType()const { return NoneType; }

	protected:

		ColorValue _color;
		unsigned int _number;
		float _intensity;
		bool _castShadow;
	}
}