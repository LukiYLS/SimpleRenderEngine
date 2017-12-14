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
		_isProjected = false;
		_shadowShader = NULL;
		//注册鼠标键盘事件，放在哪里？
		CameraControl::ptr cc = make_shared<CameraControl>(camera);
		EventManager::Inst()->registerReceiver("mouse.event", cc);
		EventManager::Inst()->registerReceiver("keyboard.event", cc);
	}
	/*

	*/
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
	/*
	true render
	*/
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
		if (!_isProjected)
		{
			projectObject(root);
			_isProjected = true;
		}

		if (_scene->getUseShadowMap())
		{
			shadowMapRender(_lights, _shadowMeshs);	
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			RenderState::setViewPort(0, 0, 800, 600);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		//use lights
		setupLights(_lights);

		////sort();

		//start render
		Skybox::ptr skybox = _scene->getSkybox();
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
	/*
	uniform need?
	*/
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
			if (type == Light::AmbientLightType)
			{

			}
			else if (type == Light::DirectionLightType)
			{				
				DirectionLight* directionLight = light->asDirectionLight();
				DirectionalLightUniform uniform_dir;

				//Vector3D direction = dir->getDirection();
				
				Vector3D position = directionLight->getWorldPosition();
				Vector3D direction =  position - Vector3D(0.0);//default target(0,0,0)
				direction.normalize();

				uniform_dir.direction = direction;
				uniform_dir.color = color * intensity;
				bool castShadow = directionLight->getCastShadow();
				uniform_dir.shadow = castShadow;
				//no nead
				uniform_dir.shadowBias = light->getShadowBias();
				uniform_dir.shadowRadius = light->getShadowRadius();
				uniform_dir.shadowMapSize = light->getShadowMapSize();

				

				directionLight->setNumber(numDirectionLight);
				directionLight->setUniform(uniform_dir);
				numDirectionLight++;

			}
			else if (type == Light::PointLightType)
			{
				PointLight* pointLight = light->asPointLight();
				PointLightUniform uniform_point;

				
				uniform_point.distance = pointLight->getDistance();
				uniform_point.position =  pointLight->getPosition();
				uniform_point.color = color * intensity;
				uniform_point.decay = pointLight->getDecay();
				bool castShadow = pointLight->getCastShadow();
				uniform_point.shadow = castShadow;

				uniform_point.shadowBias = light->getShadowBias();
				uniform_point.shadowRadius = light->getShadowRadius();
				uniform_point.shadowMapSize = light->getShadowMapSize();

				pointLight->setNumber(numPointLight);
				pointLight->setUniform(uniform_point);
				numPointLight++;
			}
			else if (type == Light::SpotLightType)
			{
				SpotLight* spotLight = light->asSpotLight();
				SpotLightUniform uniform_spot;

				Vector3D position = spotLight->getWorldPosition();
				Vector3D direction = position - Vector3D(0.0);//default target(0,0,0)
				direction.normalize();
				uniform_spot.direction = direction;
				uniform_spot.distance = spotLight->getDistance();
				uniform_spot.position = spotLight->getPosition();
				uniform_spot.color = color * intensity;
				uniform_spot.decay = (uniform_spot.distance == 0.0) ? 0.0 : spotLight->getDecay();
				uniform_spot.coneCos = cos(spotLight->getAngle());
				uniform_spot.penumbraCos = cos(spotLight->getAngle() * (1 - spotLight->getPenumbra()));
				bool castShadow = spotLight->getCastShadow();
				uniform_spot.shadow = castShadow;
				
				uniform_spot.shadowBias = light->getShadowBias();
				uniform_spot.shadowRadius = light->getShadowRadius();
				uniform_spot.shadowMapSize = light->getShadowMapSize();

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
	/*
	what's the problem,how to advance
	*/
	void RenderSystem::renderMeshs(std::vector<Mesh::ptr> meshs)
	{
		for (auto mesh : meshs)
		{
			_current_texture_unit_count = 0;
			Material::ptr material = mesh->getMaterial();
			RenderState::setMaterial(material);//before render,set render envi
			setProgram(mesh);			
			mesh->drawPrimitive();
		
		}
	}
	/*
	it's too... long, build shader and set material property,and upload 
	it must fit in all differect material, so how?
	*/
	void RenderSystem::setProgram(Mesh::ptr mesh)
	{		
		Material::ptr material = mesh->getMaterial();
		Material::MaterialType type = material->getMaterialType();
		Shader* shader = material->getShader();
		if (type == Material::ShaderMaterial) {
			//only set uniform
			if(shader==NULL)
				return;
			shader->use();
			shader->setMat4("modelMatrix", mesh->getWorldMatrix());
			shader->setMat4("viewMatrix", _camera->getViewMatrix());
			shader->setMat4("projectionMatrix", _camera->getProjectionMatrix());
			//set uniform
			return;
		}
		
		if (shader == NULL)//first frame
		{
			//Shader *shader_mat = new Shader;
			//assemble their shader,corresponding to the material

			//1.build define information
			shader = new Shader;
			std::string defineStr;

			if (_scene->getUseFog() && material->getFog())
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
				EnvMapType envmaptype = material->getEnvMapType();
				switch (envmaptype)
				{
				case CubeReflectionMapping:
				case CubeRefractionMapping:
					defineStr += "#define ENVMAP_TYPE_CUBE\n";
					break;

				case CubeUVReflectionMapping:
				case CubeUVRefractionMapping:
					defineStr += "#define ENVMAP_TYPE_CUBE_UV\n";
					break;

				case EquirectangularReflectionMapping:
				case EquirectangularRefractionMapping:
					defineStr += "#define ENVMAP_TYPE_EQUIRE\n";
					break;

				case SphericalReflectionMapping:
					defineStr += "ENVMAP_TYPE_SPHERE\n";
					break;
				default:
					break;
				}
				switch (envmaptype) {

				case CubeRefractionMapping:
				case EquirectangularRefractionMapping:
					defineStr += "#define ENVMAP_MODE_REFRACTION\n";
					break;

				}
				switch (material->getEnvMapBlendMode()) {

				case MultiplyOperation:
					defineStr += "#define ENVMAP_BLENDING_MULTIPLY\n";
					break;

				case MixOperation:
					defineStr += "#define ENVMAP_BLENDING_MIX\n";
					break;

				case AddOperation:
					defineStr += "#define ENVMAP_BLENDING_ADD\n";
					break;

				}
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
			if (_scene->getUseShadowMap())
			{
				defineStr += "#define USE_SHADOWMAP\n";
				defineStr += "#define SHADOWMAP_TYPE_PCF\n";
				//shadwomap type
			}
			
			std::string vertexStr, fragmentStr;
			switch (type)
			{
			case Material::BasicMaterial:
				vertexStr = StringHelp::readFileToStr("../../../src/Data/shader/basicMaterial.vs");
				fragmentStr = StringHelp::readFileToStr("../../../src/Data/shader/basicMaterial.fs");
				break;
			case Material::ShaderMaterial:			
				break;
			case Material::PhongMaterial:
				vertexStr = StringHelp::readFileToStr("../../../src/Data/shader/phongMaterial.vs");
				fragmentStr = StringHelp::readFileToStr("../../../src/Data/shader/phongMaterial.fs");
				break;		
			default:
				break;
			}
			std::string vertex_shader = _light_num_define + defineStr + vertexStr;
			std::string fragment_shader = _light_num_define + defineStr + fragmentStr;

			shader->load(vertex_shader.c_str(), fragment_shader.c_str());
			material->setShader(shader);

		}
		//update uniform......
		shader->use();
		shader->setMat4("modelMatrix", mesh->getWorldMatrix());
		shader->setMat4("viewMatrix", _camera->getViewMatrix());
		shader->setMat4("projectionMatrix", _camera->getProjectionMatrix());
		shader->setVec3("cameraPosition", _camera->getPosition());
		shader->setMat3("normalMatrix", MathHelper::getNormalMatrix(mesh->getWorldMatrix()));

		shader->setFloat("opacity", material->getOpacity());
		shader->setVec3("diffuse", material->getColor());
		shader->setVec3("emissive", material->getEmissive());
		shader->setVec3("specular", material->getSpecular());
		shader->setFloat("shininess", material->getShininess());

		if (_scene->getUseFog() && material->getFog())
		{
			Fog fog = _scene->getFog();
			shader->setFloat("fogNear", fog.near);
			shader->setFloat("fogFar", fog.far);
			shader->setVec3("fogColor", fog.color);
		}

		TextureUnitState::ptr map = material->getMap();
		if (map)
		{
			Matrix3D uvTransfrom = Matrix3D::Identity;
			shader->setMat3("uvTransform", uvTransfrom);
			setTexture(map);
			shader->setInt("map", _current_texture_unit_count);
			_current_texture_unit_count++;
		}
		TextureUnitState::ptr lightMap = material->getLightMap();
		if (lightMap)
		{
			setTexture(lightMap);
			shader->setInt("lightMap", _current_texture_unit_count);
			shader->setFloat("lightMapIntensity", material->getLightMapIntensity());
			_current_texture_unit_count++;
		}

		TextureUnitState::ptr aoMap = material->getAoMap();
		if (aoMap)
		{
			setTexture(aoMap);
			shader->setInt("aoMap", _current_texture_unit_count);
			shader->setFloat("aoMapIntensity", material->getAoMapIntensity());
			_current_texture_unit_count++;
		}

		TextureUnitState::ptr alphaMap = material->getAlphaMap();
		if (alphaMap)
		{
			setTexture(alphaMap);
			shader->setInt("alphaMap", _current_texture_unit_count);			
			_current_texture_unit_count++;
		}

		TextureUnitState::ptr specularMap = material->getSpecularMap();
		if (specularMap)
		{
			setTexture(specularMap);
			shader->setInt("specularMap", _current_texture_unit_count);			
			_current_texture_unit_count++;
		}
		

		TextureUnitState::ptr envMap = material->getEnvMap();
		if (envMap)
		{			
			setTexture(envMap);
			shader->setInt("envMap", _current_texture_unit_count);
			_current_texture_unit_count++;
			shader->setFloat("reflectivity", material->getReflectivity());
			shader->setFloat("refractionRatio", material->getRefractionRatio());
			float flipEnvMap = envMap->getTexture()->getTextureTarget() == TEX_TYPE_CUBE_MAP ? 1.0 : -1.0;
			shader->setFloat("flipEnvMap", flipEnvMap);
		}
		if (_scene->getUseShadowMap() && mesh->getReceiveShadow())
		{
			
		}
		//light upload...
		for (auto light : _lights)
		{
			light->upload(shader);
			if (_scene->getUseShadowMap())
				light->uploadShadow(shader, _current_texture_unit_count);
		}
			
		
	}
	/*
	something problem,
	*/
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
		//glBindTexture(target, 0);
	}
	/*
	sort object,how to avoid execute every frame?
	from root object to child node ,tranvse node according different pt
	*/
	void RenderSystem::projectObject(Object::ptr object)
	{
		//if (object->visible = false)return;
		if (object->asMesh())
		{
			Mesh::ptr mesh = (Mesh::ptr)object->asMesh();
			//if (_frustum.intersectsSphere(*mesh->getBoundSphere().get()))
			{
				Material::ptr material = mesh->getMaterial();
				if (material->getTransparent())
				{
					if (std::find(_transparentMeshs.begin(), _transparentMeshs.end(), mesh) == _transparentMeshs.end())
						_transparentMeshs.push_back(mesh);
				}
				else
				{
					if (std::find(_opaqueMehss.begin(), _opaqueMehss.end(), mesh) == _opaqueMehss.end())
						_opaqueMehss.push_back(mesh);
				}

				if (mesh->getReceiveShadow())
					_shadowMeshs.push_back(mesh);


			}	
			int a = mesh.use_count();

		}
		else if (object->asLight())
		{
			Light::ptr light = (Light::ptr)object->asLight();
			if (std::find(_lights.begin(), _lights.end(), light) == _lights.end())
				_lights.push_back((Light::ptr)light);
			if (light->getCastShadow())
			{
				if (std::find(_shadow_lights.begin(), _shadow_lights.end(), light) == _shadow_lights.end())
					_shadow_lights.push_back((Light::ptr)light);
			}
				

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
	/*
	what fuck! why place here,should i create a shadow map plugin
	*/
	void RenderSystem::shadowMapRender(std::vector<Light::ptr> lights, std::vector<Mesh::ptr> meshs)
	{
		//
		if (lights.empty())
			return;

		//use for point light ,shit!!!
		Vector3D cubeDirections[6] = { Vector3D(1, 0, 0),Vector3D(-1, 0, 0),Vector3D(0, 0, 1),
			Vector3D(0, 0, -1),Vector3D(0, 1, 0),Vector3D(0, -1, 0) };

		Vector3D cubeUps[6] = {
			Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0),
			Vector3D(0, 1, 0), Vector3D(0, 0, 1), Vector3D(0, 0, -1)
		};

		Vector4D cube2DViewPorts[6] = {
			Vector4D(), Vector4D(), Vector4D(),
			Vector4D(), Vector4D(), Vector4D()
		};

		// get shader,

		if(_shadowShader==NULL)
			_shadowShader = std::make_shared<Shader>("../../../src/Data/shader/shadowmapdepth.vs", "../../../src/Data/shader/shadowmapdepth.fs");

		Matrix4D shadowMatrix;

		for (auto light : lights)
		{
			Light::LightType type = light->getType();
			unsigned int faceCount = 1;
			Camera* shadowCamera = light->getShadowCamera();
			Vector2D mapSize = light->getShadowMapSize();
			Vector3D cameraPosition = light->getWorldPosition();
			shadowCamera->setPosition(cameraPosition);
			shadowCamera->lookAt(0.0, 0.0, 0.0);
			if (type == Light::PointLightType)
			{				

				float vpWidth = mapSize.x;
				float vpHeight = mapSize.y;
				// positive X
				cube2DViewPorts[0] = Vector4D(vpWidth * 2, vpHeight, vpWidth, vpHeight);
				// negative X
				cube2DViewPorts[1] = Vector4D(0, vpHeight, vpWidth, vpHeight);
				// positive Z
				cube2DViewPorts[2] = Vector4D(vpWidth * 3, vpHeight, vpWidth, vpHeight);
				// negative Z
				cube2DViewPorts[3] = Vector4D(vpWidth, vpHeight, vpWidth, vpHeight);
				// positive Y
				cube2DViewPorts[4] = Vector4D(vpWidth * 3, 0, vpWidth, vpHeight);
				// negative Y
				cube2DViewPorts[5] = Vector4D(vpWidth, 0, vpWidth, vpHeight);

				mapSize.x *= 4.0;
				mapSize.y *= 2.0;

				faceCount = 6;

				//Vector3D position = light->getPosition();
				//why no projection matrix,because point light shadow is 360 degree,
				shadowMatrix = Matrix4D::makeTranslation(Vector3D(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z));
			}
			else{

				/*tips: how to avoid shadow acne(because of resolution,one depth pixel correspoinding a region,so it show up
				some fragmen greater than depth, and some fragment less than depth,so we can see stagger(交错) shadow),the method
				is rise depth (add a bias value 0.0005)

				*/
				faceCount = 1;

				Matrix4D matrix(
					0.5, 0.0, 0.0, 0.0,
					0.0, 0.5, 0.0, 0.0,
					0.0, 0.0, 0.5, 0.0,
					0.5, 0.5, 0.5, 1.0
				);

				Matrix4D view = shadowCamera->getViewMatrix();

				Matrix4D projection = shadowCamera->getProjectionMatrix();

				shadowMatrix = matrix * projection * view;

			}
			//create fbo
			FrameBuffer* fb = light->getShadowFrameBuffer();
			if (fb == NULL)
			{
				fb = new FrameBuffer(mapSize.x, mapSize.y);
				light->setShadowFrameBuffer(fb);
			}
			light->setShadowMatrix(shadowMatrix);
			
			//FrameBuffer::ptr fbo = std::make_shared<FrameBuffer>();
			//fbo->createFrameBufferWithTexture(mapSize.x, mapSize.y);
			////save shadow infomation(matrix ,fbo)
			//Light::ShadowInfo shadowInfo;
			//shadowInfo.shadowMatrix = shadowMatrix;
			//shadowInfo.depthFBO = fbo;
			//light->setShadowInfo(shadowInfo);

			type == Light::PointLightType ? fb->bingForWriting(false) : fb->bingForWriting();

			for (int i = 0; i < faceCount; i++)
			{
				if (type == Light::PointLightType)
				{
					shadowCamera->lookAt(shadowCamera->getPosition(), shadowCamera->getPosition() + cubeDirections[i], cubeUps[i]);					
					RenderState::setViewPort(cube2DViewPorts[i].x, cube2DViewPorts[i].y, cube2DViewPorts[i].z, cube2DViewPorts[i].w);
				}

				for (auto mesh : meshs)
				{
					_shadowShader->use();
					_shadowShader->setMat4("modelMatrix", mesh->getWorldMatrix());
					_shadowShader->setMat4("viewMatrix", shadowCamera->getViewMatrix());
					_shadowShader->setMat4("projectionMatrix", shadowCamera->getProjectionMatrix());
					mesh->drawPrimitive();
				}				

			}
		}
		
	}
	void RenderSystem::resize(int x, int y, int width, int height)
	{
		ViewPort::ptr vp = _camera->getViewPort();
		vp->setViewport(x, y, width, height);
	}
	void RenderSystem::afterRender()
	{
		//reset ...
		//_render_mesh.clear();
		//_lights.clear();

	}		
}