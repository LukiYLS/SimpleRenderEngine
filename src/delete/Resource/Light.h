#ifndef LIGHT_H
#define LIGHT_H


#include <glm\glm.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
namespace Core
{
	enum LightType { DirectLight, PointLight, SpotLight };
	/*class Light {
	public:
		virtual void setAmbient(const glm::vec3& ambient) = 0;
		virtual void setDiffuse(const glm::vec3& diffuse) = 0;
		virtual void setSpecular(const glm::vec3& specular) = 0;
		virtual void setPosition(const glm::vec3& position) = 0;
		virtual void setDirection(const glm::vec3& direction) = 0;
		virtual void setConstantAttenuation(float constant_attenuation) = 0;
		virtual void setLinearAttenuation(float linear_attenuation) = 0;
		virtual void setQuadraticAttenuation(float quadratic_attenuation) = 0;
		virtual void setSpotCutoff(float spot_cutoff) = 0;

	};
	class DirectionLight 
		:public Light{

	};
	class PointLight
		:public Light {

	};
	class SpotLight
		:public Light{
	};*/
	 
	
	class Light {//
	public:
		typedef std::shared_ptr<Light> ptr;
	public:
		Light();
		Light(unsigned int num);
		//Light(const Light* light);
		virtual ~Light();
	public:

		//static Light::ptr createLight(Parameter::ptr paras);

		void setLightNum(int num) { _lightnum = num; }
		int getLightNum() const { return _lightnum; }

		inline void setAmbient(const glm::vec3& ambient) { _ambient = ambient; }
		inline const glm::vec3& getAmbient() const { return _ambient; }

		inline void setDiffuse(const glm::vec3& diffuse) { _diffuse = diffuse; }
		inline const glm::vec3& getDiffuse() const { return _diffuse; }

		inline void setSpecular(const glm::vec3& specular) { _specular = specular; }
		inline const glm::vec3& getSpecular() const { return _specular; }		

		inline void setPosition(const glm::vec3& position) { _position = position; }
		inline const glm::vec3& getPosition() const { return _position; }

		inline void setDirection(const glm::vec3& direction) { _direction = direction; }
		inline const glm::vec3& getDirection() const { return _direction; }

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
		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;		
		glm::vec3 _position;
		glm::vec3 _direction;

		float _innerCutOff;
		float _outerCutOff;

		float _constant;
		float _linear;
		float _quadratic;

		bool _isShadow;
		
		LightType _type;
	};

}
#endif // !LIGHT_H
