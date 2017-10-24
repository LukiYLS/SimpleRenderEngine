#include "Scene.h"
#include "PickTool.h"
#include "FrameBuffer.h"
#include "ShaderManager.h"
#include "ShadowMapPlugin.h"
namespace Core {

	void Scene::projectObject(Object* object)
	{
		//if (object->visible = false)return;
		if (object->asMesh())
		{
			Mesh* mesh = object->asMesh();
			_render_mesh.push_back(mesh);

		}
		else if (object->asLight())
		{
			Light* light = object->asLight();
			_lights.push_back(light);
		}
		else if (object->asPlugin())
		{

		}
		else if (object->asSprite())
		{

		}
		else if (object->asBillboard())
		{

		}
		else if (object->asParticleSystem())
		{

		}
		else
		{

		}
		vector<Object*> children = object->getChildren();
		if (children.size() == 0)return;
		for (auto child : children)
			projectObject(child);
	}
	void Scene::update()
	{

	}

	void Scene::render(Camera* camera)
	{	
		_root->updateMatrixWorld();
		clearTemp();
		projectObject(_root.get());	
		if (!skybox)
			skybox->render();
		else
		{
			//clear color
		}
		if (_enable_shadow)
		{
			ShadowMapPlugin* sp = new ShadowMapPlugin(_lights, _render_mesh);
			sp->render(camera);			
		}		
		//render mesh
		for (auto mesh : _render_mesh)
		{
			Shader* shader = ShaderManager::getSingleton()->getByName(mesh->getShaderName()).get();
			shader->use();
			shader->setMat4("modelMatrix", mesh->getWorldMatrix());
			shader->setMat4("viewMatrix", camera->getViewMatrix());
			shader->setMat4("projectionMatrix", camera->getProjectionMatrix());
			shader->setVec3("viewPos", camera->getPosition());
			setupLights(shader);
			if (_enable_shadow)mesh->addTexture("shadowMap");
			mesh->setupUniform(shader);
			mesh->draw(shader);
		}

		for (auto plugin : _plugins)
		{
			//plugin->render();
		}
	}
	void Scene::clearTemp()
	{
		_render_mesh.clear();
		_lights.clear();
		_plugins.clear();
	}
	void Scene::setupLights(Shader* shader)
	{
		shader->use();
		for (auto it = _lights.begin(); it != _lights.end(); it++)
		{
			if ((*it)->getType() == DirectLight)
			{
				shader->setVec3("dirLight.direction", (*it)->getDirection());
				shader->setVec3("dirLight.ambient", (*it)->getAmbient());
				shader->setVec3("dirLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("dirLight.specular", (*it)->getSpecular());
			}
			else if ((*it)->getType() == PointLight)
			{
				shader->setVec3("pointLight.position", (*it)->getPosition());
				shader->setVec3("pointLight.ambient", (*it)->getAmbient());
				shader->setVec3("pointLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("pointLight.specular", (*it)->getSpecular());

				shader->setFloat("pointLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("pointLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("pointLight.quadratic", (*it)->getQuadraticAttenuation());
			}
			else if ((*it)->getType() == SpotLight)
			{
				shader->setVec3("spotLight.position", (*it)->getPosition());
				shader->setVec3("spotLight.direction", (*it)->getDirection());
				shader->setVec3("spotLight.ambient", (*it)->getAmbient());
				shader->setVec3("spotLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("spotLight.specular", (*it)->getSpecular());

				shader->setFloat("spotLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("spotLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("spotLight.quadratic", (*it)->getQuadraticAttenuation());

				shader->setFloat("spotLight.innerCutoff", (*it)->getInnerCutoff());
				shader->setFloat("spotLight.outerCutoff", (*it)->getOuterCutoff());
			}
		}
	}
		
}