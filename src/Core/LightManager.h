#pragma once
#include "Light.h"
#include "Shader.h"
#include "../Utils/Parameter.h"
#include <string>
#include <map>
using namespace Utils;
namespace Core {

	class LightManager {
	public:
		static LightManager* getSingleton();
		Light::ptr createLight(const std::string& name, Parameter::ptr paras);
		bool turnOnLight(const std::string& name);
		bool turnOffLight(const std::string* name);

		void addLight(const std::string& name, Light* light);
		void removeLight(const std::string& name);
		void removeAll();
		Light::ptr getByName(const std::string& name);

		bool setupLights(Shader* shader);
	protected:
		static LightManager* instance;	
		typedef std::map<std::string, Light::ptr> LightMap;
		LightMap _on_lights;;
		LightMap _off_lights
	};
}