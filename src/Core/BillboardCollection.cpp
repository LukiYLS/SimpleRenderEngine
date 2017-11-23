#include "BillboardCollection.h"
namespace SRE {

	void BillboardCollection::init()
	{
		//_texture = std::make_shared<Texture>(GL_TEXTURE_2D, fileName);

		_shader = std::make_shared<Shader>("../../../src/Data/shader/billboard.vs", "../../../src/Data/shader/billboard.gs", "../../../src/Data/shader/billboard.fs");

		glGenVertexArrays(1, &_vao);

		GLuint vbo;
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, sizeof(_positions), &_positions[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);   // position

		glBindVertexArray(0);
	}

	void BillboardCollection::render(Camera* camera)//one by one problem
	{
		glBindVertexArray(_vao);
		_shader->use();
		if (!_texture)
		{
			_texture->bindTexture(0);
			_shader->setInt("texture", 0);
		}
		
		_shader->setMat4("VP", camera->getProjectionMatrix() * camera->getViewMatrix());
		_shader->setVec3("cameraPos", camera->getPosition());		
		glDrawArrays(GL_POINTS, 0, _positions.size());
	}
}