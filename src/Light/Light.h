#pragma once
#include "..\Core\Object.h"
#include "..\Core\Shader.h"
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

		virtual LightType getType()const { return NoneType; }

	protected:

		Vector3D _color;
		unsigned int _number;
		float _intensity;
		bool _castShadow;
	};
}