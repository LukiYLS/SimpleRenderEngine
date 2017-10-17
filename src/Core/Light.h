#pragma once
#include "../Math/Vector3D.h"
#include "Object.h"
#include <vector>
#include <string>
#include <map>
#include <memory>
namespace Core
{
	enum LightType { DirectLight, PointLight, SpotLight };
		 
	
	class Light 
		: public Object{
	public:
		typedef std::shared_ptr<Light> ptr;
	public:
		Light();
		Light(unsigned int num);
		//Light(const Light* light);
		virtual ~Light();
	public:		

		virtual Light* asLight() { return this; }
		virtual const Light* asLight() const { return this; }

		void setLightNum(int num) { _lightnum = num; }
		int getLightNum() const { return _lightnum; }

		inline void setAmbient(const Vector3D& ambient) { _ambient = ambient; }
		inline const Vector3D& getAmbient() const { return _ambient; }

		inline void setDiffuse(const Vector3D& diffuse) { _diffuse = diffuse; }
		inline const Vector3D& getDiffuse() const { return _diffuse; }

		inline void setSpecular(const Vector3D& specular) { _specular = specular; }
		inline const Vector3D& getSpecular() const { return _specular; }		

		inline void setPosition(const Vector3D& position) { _position = position; }
		inline const Vector3D& getPosition() const { return _position; }

		inline void setDirection(const Vector3D& direction) { _direction = direction; }
		inline const Vector3D& getDirection() const { return _direction; }

		inline void setConstantAttenuation(float constant) { _constant = constant; }
		inline const float& getConstantAttenuation() const { return _constant; }

		inline void setLinearAttenuation(float linear) { _linear = linear; }
		inline const float& getLinearAttenuation() const { return _linear; }

		inline void setQuadraticAttenuation(float quadratic) { _quadratic = quadratic; }
		inline const float& getQuadraticAttenuation()  const { return _quadratic; }

		//inline void setSpotExponent(float spot_exponent) { _spotExponet = spot_exponent; }
		//inline float getSpotExponent() const { return _spotExponet; }

		inline void setInnerCutoff(float cutoff) { _innerCutOff = cutoff; }
		inline float getInnerCutoff() const { return _innerCutOff; }

		inline void setOuterCutoff(float cutoff) { _outerCutOff = cutoff; }
		inline float getOuterCutoff() const { return _outerCutOff; }

		inline bool isShadowEffect() { return _isShadow; }

		//inline void setShiness(const float& shiness) { _shiness = shiness; }
		//inline const float& getShiness() const { return _shiness; }

		//inline void setStrength(const float& strength) { _strength = strength; }
		//inline const float& getStrength() const { return _strength; }

		inline LightType getType() const{ return _type; }
		inline void setType(LightType type) { _type = type; }

		bool enableShadow() { _isShadow = true; }
		bool disableShadow() { _isShadow = false; }
		
		//void captureLightState();

	private:
		void init();
	public:
		
	private:
		int _lightnum;
		Vector3D _ambient;
		Vector3D _diffuse;
		Vector3D _specular;		
		Vector3D _position;
		Vector3D _direction;

		float _innerCutOff;
		float _outerCutOff;

		float _constant;
		float _linear;
		float _quadratic;

		bool _isShadow;
		
		LightType _type;
	};

}

