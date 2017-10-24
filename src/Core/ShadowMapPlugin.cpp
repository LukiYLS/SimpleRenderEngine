#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Scene.h"
#include "../Math/Matrix4D.h"
#include "../Math/MathHelper.h"
namespace Core {


	void ShadowMapPlugin::render(Camera* camera)
	{
		Shader::ptr shader_depth = std::make_shared<Shader>("../../../src/Data/shader/shadow_map_depth.vs", "../../../src/Data/shader/shadow_map_depth.fs");		
		Matrix4D viewMatrix, projectMatrix;

		 _fb = std::make_shared<FrameBuffer>();	
		 _fb->createFrameBufferWithTexture(1024, 1024);
		for (auto light : _lights)
		{
			//if (!light->shadowEffect())continue;
			//只处理一个光源
			if (light->getType() == DirectLight)
			{			
				projectMatrix = Matrix4D::makeOrthographic(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 75.f);
				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(), Vector3D(0, 1, 0));//当方向光源需要产生阴影时，要设置position
			}
			else if (light->getType() == SpotLight)
			{
				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(0.0), Vector3D(0, 1, 0));
				projectMatrix = Matrix4D::makePerspective(MathHelper::radian(45.0f),(float)camera->getViewPort()->aspectRatio() , 0.1f, 50.0f);
			}

			_fb->bingForWriting();			
			shader_depth->use();
			shader_depth->setMat4("viewMatrix", viewMatrix);
			shader_depth->setMat4("projectionMatrix", projectMatrix);					
			for (auto m : _meshs)
			{
				shader_depth->setMat4("modelMatrix", m->getWorldMatrix());
				m->draw(shader_depth.get());
			}
			glViewport(0, 0, camera->getViewPort()->width(), camera->getViewPort()->height());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);			
			_fb->saveShadowMap();

			//_fb->bingForWriting(1);

			//shader->setInt("depthMap", 0);
			//quad->draw(shader);
			//shader_map->use();
			//shader_map->setMat4("lightVP", viewMatrix * projectMatrix);
			//shader_map->setInt("shadowMap", 0);
			//shader_map->setVec3("lightPos", light->getPosition());
			//shader_map->setVec3("viewPos", camera->getPosition());
			for (auto m : _meshs)
			{
				m->addUniform(new Uniform("lightPV", viewMatrix * projectMatrix, FLOAT_MAT4));
			}			
				
		}

	}
}