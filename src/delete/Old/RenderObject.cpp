#include "RenderObject.h"

namespace Core {

	void RenderObject::createBuffer()
	{
		glGenVertexArrays(1, &_vao);
		glGenBuffers(1, &_vbo);


		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

		if (!_indices.empty())
		{
			glGenBuffers(1, &_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), &_indices[0], GL_STATIC_DRAW);
		}


		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)24);

		glBindVertexArray(0);
	}

	void RenderObject::draw(Shader::ptr shader)
	{
		createBuffer();
		setShaderUniform(shader);
		GLint prim_type;
		switch (_type)
		{
		case POINT_LIST:
			prim_type = GL_POINTS;
			break;
		case LINE_LIST:
			prim_type = GL_LINES;
			break;
		case LINE_STRIP:
			prim_type = GL_LINE_STRIP;
			break;
		case TRIANGLE_LIST:
			prim_type = GL_TRIANGLES;
			break;
		case TRIANGLE_STRIP:
			prim_type = GL_TRIANGLE_STRIP;
			break;
		case TRIANGLE_FAN:
			prim_type = GL_TRIANGLE_FAN;
			break;
		default:
			prim_type = GL_TRIANGLES;
		}

		shader->use();
		glBindVertexArray(_vao);
		if (_indices.empty())
			glDrawArrays(prim_type, 0, _vertices.size());
		else
			glDrawElements(prim_type, _indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}