#include "RenderObject.h"
#include <glm\gtc\matrix_transform.hpp>
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

	void RenderObject::draw(Shader* shader)
	{
		if (!_vao)createBuffer();
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
	void RenderObject::computeNormals()
	{
		if (_vertices.size() == 0)
			return;
		if (_indices.size() > 0)
		{
			for (int i = 0; i < _indices.size(); i += 3)
			{
				int index = _indices[i];
				Vector3D v1 = Vector3D(_vertices[index].position_x, _vertices[index].position_y, _vertices[index].position_z);
				index = _indices[i + 1];
				Vector3D v2 = Vector3D(_vertices[index].position_x, _vertices[index].position_y, _vertices[index].position_z);
				index = _indices[i + 2];
				Vector3D v3 = Vector3D(_vertices[index].position_x, _vertices[index].position_y, _vertices[index].position_z);

				Vector3D v12 = v1 - v2;
				Vector3D v23 = v2 - v3;
				Vector3D normal = v23.cross(v12);
				normal.normalize();
				_vertices[_indices[i]].setNormal(normal);
				_vertices[_indices[i + 1]].setNormal(normal);
				_vertices[_indices[i + 2]].setNormal(normal);

			}
		}
		else
		{

		}
	}
	void RenderObject::computeBoundingBox()
	{

	}
	void RenderObject::computeBoundingSphere()
	{

	}
	void RenderObject::raycast(RayCaster* raycaster, AnyValue& intersects)
	{
		//with boundingbox intersect

		if (_sphere == NULL)computeBoundingSphere();
	}
}