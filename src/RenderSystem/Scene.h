#ifndef SCENE_H
#define SCENE_H
#include "Entity.h"
#include <map>
#include <string>
namespace RenderSystem {
	class Scene {
	public:
		static Scene* Inst();
		void addEntity(std::string name,Entity::ptr entity);
		void removeEntity(std::string name);		
		void render();
	private:
		static Scene* m_Inst;
		std::map<std::string, Entity::ptr> _entity_map;
	};
}
#endif // !SCENEH

