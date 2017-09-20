#include "Scene.h"
#include "../Resource/Shader.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Mesh/MeshManager.h"
#include <algorithm>
namespace Core {
	
	void Scene::addRenderMesh(const string& shader_name, const string& mesh_name)
	{
		if (ShaderManager::getSingleton()->getByName(shader_name) == NULL || MeshManager::getSingleton()->getByName(shader_name) == NULL)
			return;

		_shader_mesh[shader_name].push_back(mesh_name);

		
	}
	void Scene::removeRenderMesh(const string& mesh_name)
	{
		for (auto sm : _shader_mesh)
		{
			auto it = std::find(sm.second.begin(), sm.second.end(), mesh_name);
			if ( it!= sm.second.end())
			{
				sm.second.erase(it);
				return;
			}
		}
	}

	Light::ptr Scene::createLight(const string& name, Parameter::ptr paras)
	{
		Light::ptr light = make_shared<Light>(_lights.size()+1);	

		LightType type = paras->getValue("type");
		glm::vec3 ambient = paras->getValue("ambient");
		glm::vec3 diffuse = paras->getValue("diffuse");
		glm::vec3 specular = paras->getValue("specular");

		light->setType(type);
		light->setAmbient(ambient);
		light->setDiffuse(diffuse);
		light->setSpecular(specular);
		if (type != DirectLight)
		{
			glm::vec3 direction = paras->getValue("direction");
			light->setDirection(direction);
		}
		else
		{
			float constant = paras->getValue("constant");
			float linear = paras->getValue("linear");
			float quadratic = paras->getValue("quadratic");
			light->setConstantAttenuation(constant);
			light->setLinearAttenuation(linear);
			light->setQuadraticAttenuation(quadratic);
			glm::vec3 position = paras->getValue("position");
			light->setPosition(position);

			if (type == SpotLight)
			{
				glm::vec3 direction = paras->getValue("direction");
				light->setDirection(direction);
				float cutoff = paras->getValue("innercutoff");
				float outercutoff = paras->getValue("outercutoff");
				light->setInnerCutoff(cutoff);
				light->setOuterCutoff(outercutoff);
			}
		}
		_lights.push_back(light);
		return light;
	}
	void Scene::removeLight(const string& name)
	{

	}
	void Scene::addPlugin(const string& name, Plugin::ptr plugin)
	{
		if (_plugin_map.find(name) != _plugin_map.end())
			_plugin_map.insert(make_pair(name, plugin));
	}
	void Scene::removePlugin(const string& name)
	{
		_plugin_map.erase(name);
	}
	void Scene::render()
	{		
		if (_plugin_map.size() != 0)
		{
			for (auto p : _plugin_map)
				p.second->render(this);
		}		

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
	
	vector<Mesh::ptr> Scene::getMeshs()
	{
		vector<Mesh::ptr> meshs;
		for (auto it_shader : _shader_mesh)
		{
			for (auto it_mesh : it_shader.second)
			{
				Mesh::ptr mesh = MeshManager::getSingleton()->getByName(it_mesh);
				meshs.push_back(mesh);
			}			
		}
		return meshs;
	}

	//void Scene::pickRender(int width, int height)
	//{
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




	//}
}