#include "RenderSystem.h"
#include "RenderState.h"
#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
#include "../Utils/UtilsHelp.h"
#include "../Material/ShaderMaterial.h"
#include "../Light/DirectionLight.h"
#include <iostream>
using namespace Utils;
namespace SRE {
	
	RenderSystem::RenderSystem(Scene* scene, Camera* camera)
	{
		_scene = (Scene::ptr)scene;
		_camera = (Camera::ptr)camera;
		//注册鼠标键盘事件，放在哪里？
		CameraControl::ptr cc = make_shared<CameraControl>(camera);
		EventManager::Inst()->registerReceiver("mouse.event", cc);
		EventManager::Inst()->registerReceiver("keyboard.event", cc);
	}
	
	void RenderSystem::render()
	{	
		beforeRender();
		renderImpl();
		afterRender();
	}
	void RenderSystem::beforeRender()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	void RenderSystem::renderImpl()
	{
		if (_camera == NULL)
		{
			std::cout << "no camera" << std::endl;
			return;
		}
		Object::ptr root = _scene->getSceneRoot();
		root->updateMatrixWorld();

		if (_camera->getParent())_camera->updateMatrixWorld();
		_frustum.setFromMatrix(_camera->getProjectionMatrix()*_camera->getViewMatrix());

		projectObject(root);

		//sort();

		if (/*_enable_shadow*/)
		{
			//ShadowMapPlugin* sp = new ShadowMapPlugin(_lights, _render_mesh);
			//sp->render(camera);			
		}
		Skybox* skybox = _scene->getSkybox();
		if (skybox)
			skybox->render(_camera.get());
		else
		{
			//clear color
		}

		if (_opaqueMehss.size()>0) renderMeshs(_opaqueMehss);

		// transparent pass (back-to-front order)

		if (_transparentMeshs.size()>0) renderMeshs(_transparentMeshs);
		//_scene->render(_camera.get());
	}

	void RenderSystem::setupLights(std::vector<Light::ptr> lights)
	{
		unsigned int numDirectionLight = 0;
		unsigned int numPointLight = 0;
		unsigned int numSpotLight = 0;

		std::vector<DirectionLight*> directionLights;
		
		for (auto light : lights)
		{
			ColorValue color = light->getColor();
			float intensity = light->getIntensity();
			Light::LightType type = light->getType();
			
			if (type == Light::DirectionLightType)
			{
				//
				//DirectionLight* dir = light->asDirectionLight();
				Matrix4D worldMat = light->getWorldMatrix();
				Vector3D position(worldMat[12], worldMat[13], worldMat[14]);

				Vector3D direction = Vector3D(0.0) - position;
				direction.normalize();

				bool castShadow = light->getCastShadow();
				if (castShadow)
				{

				}

				light->setNumber(numDirectionLight);
				numDirectionLight++;			


			}
			else if (type == Light::PointLightType)
			{


				numPointLight++;
			}
			//pointLights[0].pos/color/distance/decay/shadow/shadowbias/radius/size
			//directionalLights[0].dir/color/shadow/shadowbias/radius/size
			//spotLights[0].pos/dir/color/distance/decay/conecos/penumbracos/shadow/shadowbias/radius/size
			//get light define and uniform  information
		}
	}

	void RenderSystem::renderMeshs(std::vector<Mesh::ptr> meshs)
	{
		for (auto mesh : meshs)
		{
			Material::ptr material = mesh->getMaterial();
			RenderState::setMaterial(material);//设置绘制前的环境
			setProgram(mesh);
			//setTextures();
			//然后要完成shader 获取--> uniform值设置----> 纹理设置----> draw
			Material::MaterialType mt = material->getType();
			Shader::ptr shader;
			switch (mt)
			{
			case Material::Basic:
			{
				break;
			}
			case Material::Phong:
			{

				break;
			}
			}
			//Shader::ptr shader = getMaterialShader


		}
	}
	void RenderSystem::setProgram(Mesh::ptr mesh)
	{		
		Material::ptr material = mesh->getMaterial();
		Material::MaterialType type = material->getType();
		Shader::ptr shader = material->getShader();
		if (shader == NULL)//first frame
		{
			//assemble their shader,corresponding to the material

			//1.build define information

			std::string defineStr;

			if (material->getFog())
			{
				defineStr += "#define USE_FOG\n";
			}
			if (material->getMap())
			{
				defineStr += "#define USE_MAP\n";
				//_textures.push_back(material->getMap());
			}
			if (material->getLightMap())
			{
				defineStr += "#define USE_LIGHTMAP\n";
			}
			if (material->getDisplacementMap())
			{
				defineStr += "#define USE_DISPLACEMENTMAP\n";
			}
			if (material->getEnvMap())
			{
				defineStr += "#define USE_ENVMAP\n";
			}
			if (mesh->isUseColor())
			{
				defineStr += "#define USE_COLOR\n";
			}
			if (_scene->hasShadow() && mesh->getReceiveShadow())
			{
				defineStr += "#define USE_SHADOWMAP\n";
			}
			std::string vertexStr, fragmentStr;
			switch (type)
			{
			case SRE::Material::Basic:
				vertexStr = readFileToStr("../../../src/Data/shader/basicMaterial.vs");
				fragmentStr = readFileToStr("../../../src/Data/shader/basicMaterial.fs");
				break;
			case SRE::Material::Shader:
				break;
			case SRE::Material::Phong:
				break;
			case SRE::Material::NO:
				break;
			default:
				break;
			}

		}
		switch (type) {
		case Material::Basic: {


			break;
		}

		case Material::Phong: {

			PhongMaterial* phongMaterial = material->asPhongMaterial();

			std::string shader_vertex = prefixVertex + readFileToStr("../../../src/Data/shader/phongMaterial.vs");

			std::string shader_fragment = prefixVertex + readFileToStr("../../../src/Data/shader/phongMaterial.fs");

			Shader::ptr shader = std::make_shared<Shader>();

			shader->load(shader_vertex.c_str(), shader_fragment.c_str());

			shader->setVec3("cameraPosition", _camera->getPosition());

			Matrix3D uvMatrix = Matrix3D::Identity;

			//envmap



			//shadowmap


			//shader->addUniform();

			break;

		}
		}

		Shader::ptr shader = material->getShader();
		if (NULL != shader)
		{
			//update
			shader->setMat4("viewMatrix", _camera->getViewMatrix());
			shader->setMat4("projectionMatrix", _camera->getProjectionMatrix());
			shader->setVec3("cameraPosition", _camera->getPosition());
			//其它信息如何更新
			

			return;
		}
		//先生成预定义信息，然后与不同材质对应的shader组合起来得到完整的shader，然后进行编译，最后把material的值上传至shader
		//Shader::ptr shader = getShader(material->getType());

		//shader->use();
		//组装shader
		std::string prefixVertex;// = vertex_attribute;
		std::vector<TextureUnitState::ptr> _textures;
		
		
		//if(material->)

		
		
		if (true)
		{
			//vertex_shader += "#define SHADOW_MAP\n";
		}
		std::string fragment_shader;

		//组装uniform
		Matrix4D viewMatrix = _camera->getViewMatrix();
		Matrix4D projectionMatrix = _camera->getProjectionMatrix();

		//shader->setMat4("viewMatrix", viewMatrix);
		//shader->setMat4("projectionMatrix", projectionMatrix);

		
	}
	/*
	
	*/
	void RenderSystem::setMaterial(Material::ptr material)
	{
		Shader::ptr shader = material->getShader();

		
	}
	Shader::ptr RenderSystem::getShader(Material::MaterialType type)
	{
		Shader::ptr shader;
		switch (type)
		{
		case Material::Basic:
		{
			shader = make_shared<Shader>("../../../src/Data/shader/basic.vs", "../../../src/Data/shader/basic.fs");
			break;
		}
		case Material::Phong:
		{
			shader = make_shared<Shader>("../../../src/Data/shader/phong.vs", "../../../src/Data/shader/phong.fs");
			break;
		}
		}
		return shader;
	}
	void RenderSystem::projectObject(Object::ptr object)
	{
		//if (object->visible = false)return;
		if (object->asMesh())
		{
			Mesh* mesh = object->asMesh();
			if (_frustum.intersectsSphere(*mesh->getBoundSphere().get()))
			{
				Material::ptr material = mesh->getMaterial();
				if (material->getTransparent())
				{
					_transparentMeshs.push_back((Mesh::ptr)mesh);
				}
				else
				{
					_opaqueMehss.push_back((Mesh::ptr)mesh);
				}
			}				

		}
		else if (object->asLight())
		{
			Light* light = object->asLight();
			_lights.push_back((Light::ptr)light);
			if (light->getCastShadow())
				_shadow_lights.push_back((Light::ptr)light);

		}
		else if (object->asPlugin())
		{

		}
		else if (/*object->asSprite()*/)
		{

		}
		else if (object->asBillboardCollection())
		{
			//BillboardCollection* bbc = object->asBillboardCollection();
			//bbc->init();
			//_plugins.push_back(bbc);
		}
		else if (/*object->asParticleSystem()*/)
		{

		}
		else
		{

		}
		unsigned int count = object->getChildCount();
		for (int i = 0; i < count; i++)
		{
			Object::ptr child = object->getChild(i);
			projectObject(child);
		}	
	}
	void RenderSystem::resize(int x, int y, int width, int height)
	{
		ViewPort::ptr vp = _camera->getViewPort();
		vp->setViewport(x, y, width, height);
	}
	void RenderSystem::afterRender()
	{
		//_render_mesh.clear();
		//_lights.clear();

	}		
}