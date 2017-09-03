#ifndef SCENE_H
#define SCENE_H
#include "Entity.h"
#include "Light.h"
#include "Camera.h"
#include <map>
#include <string>
namespace Basic {
	class Scene {//要是camera和light也能继承entity就很完美？
	public:
		static Scene* Inst();
		void addEntity(std::string name, Entity::ptr entity);
		void removeEntity(std::string name);
		void addLight(Light::ptr light) { _lights.push_back(light); }
		void update(float time);//
		void pickRender(int width, int height);//设置color，render to buffer，read buffer，
		
		void render(Camera::ptr camera, float currentTime);
	private:
		static Scene* m_Inst;
		std::map<std::string, Entity::ptr> _entity_map;
		//std::map<LightType, Light::ptr> _light_map;
		std::vector<Light::ptr> _lights;
		//std::vector<Entity::ptr> _entity_vec;
		//std::vector<glm::vec4> _color_vec;
	};
}
#endif // !SCENEH

