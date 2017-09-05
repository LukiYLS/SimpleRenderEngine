#include "Scene.h"
#include "PickTool.h"
#include "FrameBuffer.h"
#include "Shader.h"
namespace Basic {
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
	void Scene::render(RenderParams* params)
	{
		//遍历所有的灯光，作用于所有的mesh，后期考虑defer render加入	
		update(params->currTime());
		for (auto it = _entity_map.begin(); it != _entity_map.end(); it++)
		{
			it->second->setupLights(_lights);
			it->second->draw(params);
		}
			
	}
	void Scene::update(float time)
	{
		//场景有哪些需要更新的？
	}

	void Scene::pickRender(int width, int height)
	{
		//render to buffer
		/*int index = 0;
		for (auto it = _entity_map.begin(); it != _entity_map.end(); it++, index++)
		{
			glm::vec4 color = GetColorByIndex(index);
			_color_vec.push_back(color);
			_entity_vec.push_back(it->second);
		}
		FrameBuffer fbo;
		fbo.createFrameBufferWithTexture(width, height);

		fbo.bindFrameBuffer(true);


		Shader shader("", "");
		
		shader.use();
		int i = 0;
		for (auto it = _entity_map.begin(); it != _entity_map.end(); it++,i++)
		{
			shader.setVec4("color", _color_vec[i]);
			it->second->draw();
		}

		fbo.bindFrameBuffer();
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		int result = GetPickedColorIndexUnderMouse();*/




	}
}