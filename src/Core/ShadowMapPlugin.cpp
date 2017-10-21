#include "ShadowMapPlugin.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Scene.h"
#include "../Math/Matrix4D.h"
#include "../Math/MathHelper.h"
namespace Core {


	void ShadowMapPlugin::render(Camera* camera)
	{
		Shader::ptr shader_depth = std::make_shared<Shader>("../../../src/Data/shader/shadow_map_depth.vs", "../../../src/Data/shader/shadow_map_depth.fs");
		Shader::ptr shader_map = std::make_shared<Shader>("../../../src/Data/shader/shadow_map.vs", "../../../src/Data/shader/shadow_map.fs");
		Matrix4D viewMatrix, projectMatrix;

		FrameBuffer::ptr fb = std::make_shared<FrameBuffer>();	
		fb->createFrameBufferWithTexture(camera->getViewPort()->width(), camera->getViewPort()->height());		
		for (auto light : _lights)
		{

			if (light->getType() == DirectLight)
			{			
				projectMatrix = Matrix4D::makeOrthographic(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(), Vector3D(0, 1, 0));//当方向光源需要产生阴影时，要设置position
			}
			else if (light->getType() == SpotLight)
			{
				viewMatrix = Matrix4D::lookAt(light->getPosition(), Vector3D(), Vector3D(0, 1, 0));
				projectMatrix = Matrix4D::makePerspective(MathHelper::radian(45.0f),(float)camera->getViewPort()->aspectRatio() , 0.1f, 50.0f);
			}

			fb->bindFrameBufferTexture(0);
			shader_depth->use();
			shader_depth->setMat4("viewMatrix", viewMatrix);
			shader_depth->setMat4("projectMatrix", projectMatrix);
			glClear(GL_DEPTH_BUFFER_BIT);			
			for (auto m : _meshs)
			{
				m->draw(shader_depth.get());
			}
			shader_map->use();
			shader_map->setMat4("lightVP", viewMatrix * projectMatrix);
			shader_map->setInt("shadowMap", 0);
			shader_map->setVec3("lightPos", light->getPosition());
			shader_map->setVec3("viewPos", camera->getPosition());
			for (auto m : _meshs)
			{
				m->draw(shader_depth.get());
			}			
				
		}

	}
}