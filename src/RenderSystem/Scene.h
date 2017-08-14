#ifndef SCENE_H
#define SCENE_H
#include "Entity.h"
#include "Light.h"
#include <map>
#include <string>
namespace RenderSystem {
	class Scene {
	public:
		static Scene* Inst();
		void addEntity(std::string name, Entity::ptr entity);
		void removeEntity(std::string name);
		void addLight();
		void setCamera();
		void update();//

		void render();
	private:
		static Scene* m_Inst;
		std::map<std::string, Entity::ptr> _entity_map;
		std::map<LightType, Light::ptr> _light_map;
	};
}
#endif // !SCENEH

