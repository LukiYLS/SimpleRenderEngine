#include "ShadowEffect.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FrameBuffer.h"
#include "Mesh.h"
namespace Core {

	void ShadowEffect::render(std::vector<Entity::ptr> entitys)
	{

		Shader::ptr shader_depth = std::make_shared<Shader>("", "");
		Shader::ptr shader_map = std::make_shared<Shader>("", "");
		glm::mat4 viewMatrix, projectMatrix;

		FrameBuffer::ptr fb = std::make_shared<FrameBuffer>();
		fb->createFrameBufferWithTexture(_viewPort->width(), _viewPort->height());
		for (auto light : _lights)
		{

			if (light->getType() == DirectLight)
			{
				//position如何确定
				projectMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
				viewMatrix = glm::lookAt(light->getPosition(), glm::vec3(), glm::vec3(0, 1, 0));//当方向光源需要产生阴影时，要设置position
			}
			else if (light->getType() == SpotLight)
			{
				viewMatrix = glm::lookAt(light->getPosition(), glm::vec3(), glm::vec3(0, 1, 0)); 
				projectMatrix = glm::perspective(glm::radians(45.0f),(float)_viewPort->aspectRatio() , 0.1f, 10.0f);
			}

			fb->bindFrameBufferTexture(0);
			shader_depth->setMat4("viewMatrix", viewMatrix);
			shader_depth->setMat4("projectMatrix", projectMatrix);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (auto e : entitys)
			{
				e->setProgram(shader_depth);
				e->draw();
			}
			shader_map->setMat4("lightVP", viewMatrix * projectMatrix);
			shader_map->setInt("depth_map", 0);
			for (auto e : entitys)
			{
				e->setProgram(shader_depth);
				e->draw();
			}
			
				//e->draw();
		}

	}
}