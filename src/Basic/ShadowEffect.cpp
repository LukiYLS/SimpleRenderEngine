#include "ShadowEffect.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "FrameBuffer.h"
namespace Basic {

	void ShadowEffect::render(std::vector<Entity::ptr> entitys)
	{

		Shader::ptr shader = std::make_shared<Shader>("", "");

		glm::mat4 viewMatrix, projectMatrix;

		FrameBuffer::ptr fb = std::make_shared<FrameBuffer>();
		//fb->createFrameBufferWithTexture()
		for (auto light : _lights)
		{

			if (light->getType() == DirectLight)
			{
				//position如何确定
				projectMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
				//viewMatrix = glm::lookAt()
			}
			else if (light->getType() == SpotLight)
			{

				//projectMatrix = glm::perspective()
			}

			fb->bindFrameBufferTexture(0);
			shader->setMat4("viewMatrix", viewMatrix);
			shader->setMat4("projectMatrix", projectMatrix);
			glClear(GL_DEPTH_BUFFER_BIT);
			for (auto e : entitys)
				//e->draw();
		}

	}
}