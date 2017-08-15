#include "Scene.h"
#include "PickTool.h"
#include "FrameBuffer.h"
namespace RenderSystem {
	Scene* Scene::m_Inst(0);
	Scene* Scene::Inst()
	{
		if (m_Inst == nullptr)
			m_Inst = new Scene;
		return m_Inst;
	}
	void Scene::addEntity(std::string name, Entity::ptr entity)
	{
		//if(_entity_map.find(name)!=_entity_map.end())
		_entity_map[name] = entity;
	}
	void Scene::removeEntity(std::string name)
	{
		if (_entity_map.find(name) != _entity_map.end())
			_entity_map.erase(name);
	}
	void Scene::render()
	{
		for (auto it = _entity_map.begin(); it != _entity_map.end(); it++)
			it->second->draw();
	}

	void Scene::pickRender()
	{
		//render to buffer
		int index = 0;
		for (auto it = _entity_map.begin(); it != _entity_map.end(); it++, index++)
		{
			glm::vec4 color = GetColorByIndex(index);
			_entity_vec.push_back(it->second);
		}
		FrameBuffer fbo;
		fbo.createFrameBufferWithTexture()

	}
}