#include "RenderObject.h"
#include "HardwareBuffer\HardwareBufferManager.h"
#include <glm\gtc\matrix_transform.hpp>
namespace SRE {

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
	void RenderObject::drawPrimitive()
	{
		if (!_vertex_data)
			return;

		VertexElementList elements = _vertex_data->getVertexDeclaration()->getElements();
		for (VertexElementList::iterator iter = elements.begin(); iter != elements.end(); iter++)
		{
			VertexElement::ptr elem = (*iter);
			unsigned int vert_start = _vertex_data->getVertexStart();
			unsigned short source = elem->getSource();
			HardwareVertexBuffer::ptr vertex_buffer = _vertex_data->getVertexBufferBinding()->getBuffer(source);
			if (!vertex_buffer)
				continue;		

			void *buffer_data = 0;

			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer->getBufferID());

			buffer_data = (void*)elem->getOffset();
			if (vert_start)
			{
				buffer_data = static_cast<char*>(buffer_data) + vert_start * vertex_buffer->getVertexSize();
			}

			Vertex_Element_Semantic sem = elem->getSemantic();

			switch (sem)
			{
			case VES_POSITION:
				glVertexAttribPointer(0,
					VertexElement::getTypeCount(elem->getType()), HardwareBufferManager::getGLType(elem->getType()),
					GL_FALSE, vertex_buffer->getVertexSize(), buffer_data);
				glEnableVertexAttribArray(0);
				break;

			case VES_NORMAL:
				glVertexAttribPointer(1,
					VertexElement::getTypeCount(elem->getType()), HardwareBufferManager::getGLType(elem->getType()),
					GL_FALSE, vertex_buffer->getVertexSize(), buffer_data);
				glEnableVertexAttribArray(1);
				break;

			case VES_DIFFUSE:
				glVertexAttribPointer(2,
					VertexElement::getTypeCount(elem->getType()), HardwareBufferManager::getGLType(elem->getType()),
					GL_FALSE, vertex_buffer->getVertexSize(), buffer_data);
				glEnableVertexAttribArray(2);
				break;

			case VES_TEXTURE_COORDINATES:
				glVertexAttribPointer(3,
					VertexElement::getTypeCount(elem->getType()), HardwareBufferManager::getGLType(elem->getType()),
					GL_FALSE, vertex_buffer->getVertexSize(), buffer_data);
				glEnableVertexAttribArray(3);
				break;

			default:
				break;
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
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

		if (_index_data)
		{		
			void *buffer_data = 0;
			HardwareIndexBuffer::ptr index_buf = _index_data->getHardwareIndexBuffer();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buf->getBufferID());
			buffer_data = (char*)NULL + _index_data->getIndexStart() * index_buf->getIndexSize();
			GLenum index_type = (index_buf->getIndexType() == HardwareIndexBuffer::IT_16BIT) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;			
			glDrawElements(prim_type, _index_data->getIndexCount(), index_type, buffer_data);
		}
		else
		{
			glDrawArrays(prim_type, 0, _vertex_data->getVertexCount());
		}
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
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
	BoundingSphere::ptr RenderObject::getBoundSphere()
	{
		if (!_sphere)
		{
			computeBoundingSphere();
		}
		return _sphere;
	}
	void RenderObject::raycast(RayCaster* raycaster, AnyValue& intersects)
	{
		//with boundingbox intersect

		if (_sphere == NULL)computeBoundingSphere();
	}
}