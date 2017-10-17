#include "Scene.h"
#include "PickTool.h"
#include "FrameBuffer.h"
#include "Shader.h"
namespace Core {
	
	/*void update(Group* group)
	{
		if()
	}
	void Scene::update()
	{
		for (int i = 0; i < _root->getNumChildren(); i++)
		{
			if (_root->getChild(i)->asGroup() != 0)
			{

				update();
			}
			else if (_root->getChild(i)->asNode() != 0)
			{
				SubMesh* sub = _root->getChild(i)->asSubMesh();
				
			}
			
		}
	}
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
			if (it != sm.second.end())
			{
				sm.second.erase(it);
				return;
			}
		}
	}

	Light::ptr Scene::createLight(const string& name, Parameter::ptr paras)
	{
		Light::ptr light = make_shared<Light>(_lights.size() + 1);

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
	void Scene::render()
	{
		if (_camera == nullptr)
		{
			printf("This scene has not camera");
			return;
		}
		//this.updateMatrix---> for updata matrix while form farther node
		//camera.updateMatrix
		//frustum.setFromMatrix();
		//projectObject();---->for differ type of object
		//shadowMap.render()
		//setupLights()------>light to shader
		//setRenderTarget()
		//if(background==null || ==Color)setClearColor
		//else renderBackgound
		//render opaqueobjects
		//render transparentObjects
		//render spriteplugin
		//after render 
		if (!_plugin_map.empty())
		{
			for (auto p : _plugin_map)
				p.second->render(this);
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
			//meshs.push_back(it.second);
		}
		return meshs;
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