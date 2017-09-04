#ifndef LIGHT_H
#define LIGHT_H


#include <glm\glm.hpp>
#include <vector>
#include <string>
#include <map>
#include <memory>
namespace Basic
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
	 
	
	class Light {//有没有必要设计成抽象类？
	public:
		typedef std::shared_ptr<Light> ptr;
	public:
		Light();
		Light(unsigned int num);
		//Light(const Light* light);
		virtual ~Light();
	public:

		void setLightNum(int num) { _lightnum = num; }
		int getLightNum() const { return _lightnum; }

		inline void setColor(const glm::vec3& color) { _color = color; }
		inline const glm::vec3& getColor() const { return _color; }

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

		inline void setConstantAttenuation(float constant_attenuation) { _constantAttenuation = constant_attenuation; }
		inline const float& getConstantAttenuation() const { return _constantAttenuation; }

		inline void setShiness(const float& shiness) { _shiness = shiness; }
		inline const float& getShiness() const { return _shiness; }

		inline void setStrength(const float& strength) { _strength = strength; }
		inline const float& getStrength() const { return _strength; }

		inline void setLinearAttenuation(float linear_attenuation) { _linearAttenuation = linear_attenuation; }
		inline const float& getLinearAttenuation() const { return _linearAttenuation; }

		inline void setQuadraticAttenuation(float quadratic_attenuation) { _linearAttenuation = quadratic_attenuation; }
		inline const float& getQuadraticAttenuation()  const { return _linearAttenuation; }

		inline void setSpotExponent(float spot_exponent) { _spotExponet = spot_exponent; }
		inline float getSpotExponent() const { return _spotExponet; }

		inline void setSpotCutoff(float spot_cutoff) { _spotCosCutoff = spot_cutoff; }
		inline float getSpotCutoff() const { return _spotCosCutoff; }

		inline LightType getType() const{ return _type; }
		inline void setType(LightType type) { _type = type; }
		//void captureLightState();

	private:
		void init();
	private:
		int _lightnum;
		glm::vec3 _color;
		glm::vec3 _ambient;
		glm::vec3 _diffuse;
		glm::vec3 _specular;
		glm::vec3 _position;
		glm::vec3 _direction;

		float _shiness;
		float _strength;
		float _constantAttenuation;
		float _linearAttenuation;
		float _quatricAttenuaion;

		glm::vec3 _coneDirection;
		float _spotCosCutoff;
		float _spotExponet;

		LightType _type;
	};

}
#endif // !LIGHT_H
