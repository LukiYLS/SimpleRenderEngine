#include "Scene.h"
#include "PickTool.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "MeshManager.h"
#include "ShaderManager.h"
namespace Core {
	
	void Scene::addRenderMesh(const string& shader_name, const string& mesh_name)
	{
		if (ShaderManager::getSingleton()->getByName(shader_name) == NULL || MeshManager::getSingleton()->getByName(shader_name) == NULL)
			return;

		_shader_mesh[shader_name].push_back(mesh_name);

		
	}
	void Scene::removeRenderMesh(const string& mesh_name)
	{
		
	}
	void Scene::render()
	{		
		if (_plugin_map.size() != 0)
		{
			for (auto p : _plugin_map)
				p.second->render((ptr)this);
		}
		else 
		{

			for (auto m : _shader_mesh)
			{
				
				Shader::ptr shader = ShaderManager::getSingleton()->getByName(m.first);
				shader->use();
				shader->setMat4("view", _camera->getViewMatrix());
				shader->setMat4("projection", _camera->getProjectionMatrix());
				for (int i = 0; i < m.second.size(); ++i)
				{
					string mesh_name = m.second.at(i);
					Mesh::ptr mesh = MeshManager::getSingleton()->getByName(mesh_name);
					mesh->setLights(_lights);
					mesh->draw(shader);
				}
				
			}
		}
	}
	
	vector<Mesh::ptr> Scene::getMeshs()
	{
		vector<Mesh::ptr> meshs;
		for (auto it : _shader_mesh)
		{
			meshs.push_back
		}
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