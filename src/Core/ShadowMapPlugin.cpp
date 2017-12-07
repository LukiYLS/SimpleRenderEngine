#include "ShadowMapPlugin.h"
#include "ShaderManager.h"
#include "Mesh.h"
#include "Scene.h"
#include "../Math/Matrix4D.h"
#include "../Math/MathHelper.h"

namespace SRE {


	void ShadowMapPlugin::render(Camera* camera)
	{
//		Shader::ptr shader_depth = std::make_shared<Shader>("../../../src/Data/shader/shadow_map_depth.vs", "../../../src/Data/shader/shadow_map_depth.fs");		
//		Matrix4D viewMatrix, projectMatrix;
//
//		 _fb = std::make_shared<FrameBuffer>();	
//		 _fb->createFrameBufferWithTexture(1024, 1024);
//		for (auto light : _lights)
//		{
//			//if (!light->shadowEffect())continue;
//			//只处理一个光源
//			
//			if (light->getType() == Light::DirectLight)
//			{			
//				projectMatrix = Matrix4D::makeOrthographic(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.f);
//				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(), Vector3D(0, 1, 0));//当方向光源需要产生阴影时，要设置position
//			}
//			else if (light->getType() == Light::SpotLight)
//			{
//				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(0.0), Vector3D(0, 1, 0));
//				projectMatrix = Matrix4D::makePerspective(MathHelper::radian(45.0f),(float)camera->getViewPort()->aspectRatio() , 1.f, 100.0);
//			}
//			
////			Matrix4D m = projectMatrix * viewMatrix;
////			Vector4D v = m.getTranspose()*Vector4D(0.0, 0.0, -20.0, 1.0);
////			glm::mat4 view = glm::lookAt(Vector3D(light->getPosition().x, light->getPosition().y, light->getPosition().z), Vector3D(0, 0, 0), Vector3D(0, 1, 0));
////			glm::mat4 proj = glm::perspectiveFov((float)glm::radians(45.0), (float)camera->getViewPort()->width(), (float)camera->getViewPort()->height(), 1.0f, 100.0f);
////			glm::mat4 v1 = proj* view ;
////			glm::vec4 vv = glm::transpose(v1) * glm::vec4(0.0, 0.0, -20.0, 1.0);
////			_fb->bingForWriting();			
//			shader_depth->use();
//			shader_depth->setMat4("viewMatrix", viewMatrix);
//			shader_depth->setMat4("projectionMatrix", projectMatrix);					
//			for (auto m : _meshs)
//			{
//				shader_depth->setMat4("modelMatrix", m->getWorldMatrix());
//				m->draw(shader_depth.get());
//			}
//			glViewport(0, 0, camera->getViewPort()->width(), camera->getViewPort()->height());
//			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//			glBindFramebuffer(GL_FRAMEBUFFER, 0);			
//			_fb->saveShadowMap();
//
//			//_fb->bingForWriting(1);
//
//			//shader->setInt("depthMap", 0);
//			//quad->draw(shader);
//			//shader_map->use();
//			//shader_map->setMat4("lightVP", viewMatrix * projectMatrix);
//			//shader_map->setInt("shadowMap", 0);
//			//shader_map->setVector3D("lightPos", light->getPosition());
//			//shader_map->setVector3D("viewPos", camera->getPosition());
//			for (auto m : _meshs)
//			{
//				//m->addUniform(new Uniform("lightPV", projectMatrix * viewMatrix, FLOAT_MAT4));
//			}			
				
		}

	
}