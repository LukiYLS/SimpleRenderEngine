#include "RenderSystem.h"
#include "RenderState.h"
#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include "../Utils/CamerControl.h"
#include "../Utils/Event.h"
#include "../Utils/UtilsHelp.h"
#include "../Material/ShaderMaterial.h"
#include "../Light/DirectionLight.h"
#include "../Light/PointLight.h"
#include "../Light/SpotLight.h"
#include "../Math/MathHelper.h"
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
		_current_texture_unit_count = 0;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
	}
	void RenderSystem::renderImpl()
	{
		if (_camera == NULL)
		{
			std::cout << "SB,There is no camera!" << std::endl;
			return;
		}
		Object::ptr root = _scene->getSceneRoot();
		root->updateMatrixWorld();

		if (_camera->getParent())_camera->updateMatrixWorld();
		_frustum.setFromMatrix(_camera->getProjectionMatrix()*_camera->getViewMatrix());

		//Sort each object
		projectObject(root);

		if (_scene->getUseShadowMap())
		{

		}
		//use lights
		setupLights(_lights);

		//sort();

		//start render
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
			Vector3D color = light->getColor();
			float intensity = light->getIntensity();
			Light::LightType type = light->getType();
			
			if (type == Light::DirectionLightType)
			{				
				DirectionLight* directionLight = light->asDirectionLight();
				DirectionalLightUniform uniform_dir;

				//Vector3D direction = dir->getDirection();
				
				Vector3D position = directionLight->getWorldPosition();
				Vector3D direction = Vector3D(0.0) - position;//default target(0,0,0)
				direction.normalize();

				uniform_dir.direction = direction;
				uniform_dir.color = color * intensity;
				bool castShadow = directionLight->getCastShadow();
				uniform_dir.shadow = castShadow;
				if (castShadow)
				{

				}

				directionLight->setNumber(numDirectionLight);
				directionLight->setUniform(uniform_dir);
				numDirectionLight++;

			}
			else if (type == Light::PointLightType)
			{
				PointLight* pointLight = light->asPointLight();
				PointLightUniform uniform_point;

				
				uniform_point.distance = pointLight->getDistance();
				uniform_point.position = pointLight->getPosition();
				uniform_point.color = color * intensity;
				uniform_point.decay = (uniform_point.distance == 0.0) ? 0.0 : pointLight->getDecay();
				bool castShadow = pointLight->getCastShadow();
				uniform_point.shadow = castShadow;
				if (castShadow)
				{

				}
				pointLight->setNumber(numPointLight);
				pointLight->setUniform(uniform_point);
				numPointLight++;
			}
			else if (type == Light::SpotLightType)
			{
				SpotLight* spotLight = light->asSpotLight();
				SpotLightUniform uniform_spot;

				uniform_spot.direction = spotLight->getDirection();
				uniform_spot.distance = spotLight->getDistance();
				uniform_spot.position = spotLight->getPosition();
				uniform_spot.color = color * intensity;
				uniform_spot.decay = (uniform_spot.distance == 0.0) ? 0.0 : spotLight->getDecay();
				uniform_spot.coneCos = cos(spotLight->getAngle());
				uniform_spot.penumbraCos = cos(spotLight->getAngle() * (1 - spotLight->getPenumbra()));
				bool castShadow = spotLight->getCastShadow();
				uniform_spot.shadow = castShadow;
				if (castShadow)
				{

				}
				spotLight->setNumber(numSpotLight);
				spotLight->setUniform(uniform_spot);
				numSpotLight++;
			}

		}
		string dir_define = "#define NUM_DIR_LIGHTS ";
		string point_define = "#define NUM_POINT_LIGHTS ";
		string spot_define = "#define NUM_SPOT_LIGHTS ";

		_light_num_define = dir_define + StringHelp::Int2String(numDirectionLight) + "\n" +
			point_define + StringHelp::Int2String(numPointLight) + "\n" +
			spot_define + StringHelp::Int2String(numSpotLight) + "\n";

	}

	void RenderSystem::renderMeshs(std::vector<Mesh::ptr> meshs)
	{
		for (auto mesh : meshs)
		{
			Material::ptr material = mesh->getMaterial();
			RenderState::setMaterial(material);//before render,set render envi
			setProgram(mesh);
			//setTextures();
			//然后要完成shader 获取--> uniform值设置----> 纹理设置----> draw
			//prepare shader 
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
		Material::MaterialType type = material->getMaterialType();
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
			if (material->getSpecularMap())
			{
				defineStr += "#define USE_SPECULARMAP\n";
			}
			if (material->getAlphaMap())
			{
				defineStr += "#define USE_AlPHAMAP\n";
			}
			if (material->getAoMap())
			{
				defineStr += "#define USE_AOMAP\n";
			}

			if (mesh->isUseColor())
			{
				defineStr += "#define USE_COLOR\n";
			}
			if (_scene->getUseShadowMap() && mesh->getReceiveShadow())
			{
				defineStr += "#define USE_SHADOWMAP\n";
			}
			
			std::string vertexStr, fragmentStr;
			switch (type)
			{
			case SRE::Material::Basic:
				vertexStr = StringHelp::readFileToStr("../../../src/Data/shader/basicMaterial.vs");
				fragmentStr = StringHelp::readFileToStr("../../../src/Data/shader/basicMaterial.fs");
				break;
			case SRE::Material::Shader:			
				break;
			case SRE::Material::Phong:
				vertexStr = StringHelp::readFileToStr("../../../src/Data/shader/phongMaterial.vs");
				fragmentStr = StringHelp::readFileToStr("../../../src/Data/shader/phongMaterial.fs");
				break;
			case SRE::Material::NO:
				break;
			default:
				break;
			}
			std::string vertex_shader = _light_num_define + defineStr + vertexStr;
			std::string fragment_shader = _light_num_define + defineStr + fragmentStr;

			shader->load(vertex_shader.c_str(), fragment_shader.c_str());

		}
		//update uniform......
		shader->use();
		shader->setMat4("modelMatrix", mesh->getWorldMatrix());
		shader->setMat4("viewMatrix", _camera->getViewMatrix());
		shader->setMat4("projectionMatrix", _camera->getProjectionMatrix());
		shader->setVec3("cameraPosition", _camera->getPosition());
		shader->setMat3("normalMatrix", MathHelper::getNormalMatrix(mesh->getWorldMatrix()));

		TextureUnitState::ptr map = material->getMap();
		if (map)
		{
			Matrix3D uvTransfrom = map->getUvTransfrom();
			shader->setMat3("uvTransfrom", uvTransfrom);
			setTexture(map);
			shader->setInt("map", _current_texture_unit_count);
			_current_texture_unit_count++;
		}
		TextureUnitState::ptr envMap = material->getEnvMap();
		if (envMap)
		{
			//float refractionRatio = 
			setTexture(envMap);
			shader->setInt("envMap", _current_texture_unit_count);
			_current_texture_unit_count++;
		}
		//light upload...
		for (auto light : _lights)
			light->upload(shader);

		//
		mesh->drawPrimitive();
		switch (type) {
		case Material::Basic: {


			break;
		}

		case Material::Phong: {

			/*PhongMaterial* phongMaterial = material->asPhongMaterial();

			std::string shader_vertex = prefixVertex + readFileToStr("../../../src/Data/shader/phongMaterial.vs");

			std::string shader_fragment = prefixVertex + readFileToStr("../../../src/Data/shader/phongMaterial.fs");

			Shader::ptr shader = std::make_shared<Shader>();

			shader->load(shader_vertex.c_str(), shader_fragment.c_str());

			shader->setVec3("cameraPosition", _camera->getPosition());

			Matrix3D uvMatrix = Matrix3D::Identity;*/

			//envmap



			//shadowmap


			//shader->addUniform();

			break;

		}
		}

		//Shader::ptr shader = material->getShader();
		if (NULL != shader)
		{
			//update
			
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
	void RenderSystem::setTexture(TextureUnitState::ptr texture)
	{
		UVWAddressingMode mode = texture->getTextureAddressingMode();
		FilterOptions minFilter = texture->getTextureFiltering(FT_MIN);
		FilterOptions magFilter = texture->getTextureFiltering(FT_MAG);
		FilterOptions mipFilter = texture->getTextureFiltering(FT_MIP);

		GLenum target = texture->getTexture()->getTextureTarget();

		//there is some problem
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		
		glTexParameteri(target, GL_TEXTURE_WRAP_S, mode.u);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, mode.v);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, mode.w);

		texture->getTexture()->bindTextureUnit(_current_texture_unit_count);
		
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
		else if (true/*object->asSprite()*/)
		{

		}
		else if (object->asBillboardCollection())
		{
			//BillboardCollection* bbc = object->asBillboardCollection();
			//bbc->init();
			//_plugins.push_back(bbc);
		}
		else if (true/*object->asParticleSystem()*/)
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