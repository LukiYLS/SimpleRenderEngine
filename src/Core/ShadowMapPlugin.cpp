#include "ShadowMapPlugin.h"
#include "Shader.h"


#include "Mesh.h"
#include "Scene.h"
namespace Core {


	void ShadowMapPlugin::render()
	{
		/*Shader::ptr shader_depth = std::make_shared<Shader>("../Data/shader/shader_map_depth.vs", "../Data/shader/shader_map_depth.fs");
		Shader::ptr shader_map = std::make_shared<Shader>("../Data/shader/shader_map.vs", "../Data/shader/shader_map.fs");
		glm::mat4 viewMatrix, projectMatrix;

		FrameBuffer::ptr fb = std::make_shared<FrameBuffer>();
		Camera::ptr camera = scene->getCamera();
		fb->createFrameBufferWithTexture(camera->getViewPort()->width(), camera->getViewPort()->height());
		vector<Light::ptr> lights = scene->getLights();
		for (auto light : lights)
		{

			if (light->getType() == DirectLight)
			{			
				projectMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
				viewMatrix = glm::lookAt(light->getPosition(), glm::vec3(), glm::vec3(0, 1, 0));//当方向光源需要产生阴影时，要设置position
			}
			else if (light->getType() == SpotLight)
			{
				viewMatrix = glm::lookAt(light->getPosition(), glm::vec3(), glm::vec3(0, 1, 0)); 
				projectMatrix = glm::perspective(glm::radians(45.0f),(float)camera->getViewPort()->aspectRatio() , 0.1f, 10.0f);
			}

			fb->bindFrameBufferTexture(0);
			shader_depth->setMat4("viewMatrix", viewMatrix);
			shader_depth->setMat4("projectMatrix", projectMatrix);
			glClear(GL_DEPTH_BUFFER_BIT);
			vector<Mesh::ptr> meshs = scene->getMeshs();
			for (auto m : meshs)
			{
				m->draw(shader_depth);
			}
			shader_map->setMat4("lightVP", viewMatrix * projectMatrix);
			shader_map->setInt("depth_map", 0);
			for (auto m : meshs)
			{
				m->draw(shader_depth);
			}			
				
		}*/

	}
}