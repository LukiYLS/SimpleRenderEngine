#pragma once
#include <memory>
#include "../Shader.h"

namespace Core {


	struct Vertex
	{
		float position_x, position_y, position_z;
		float normal_x, normal_y, normal_z;
		float texcoord_x, texcoord_y;
		float tangent_x, tangent_y, tangent_z;
		float bitangent_x, bitangent_y, bitangent_z;
		Vertex()
		{
			position_x = position_y = position_z = 0;
			normal_x = normal_y = normal_z = 0;
			texcoord_x = texcoord_y = 0;
		}
		Vertex(float px, float py, float pz)
		{
			position_x = px;		position_y = py;		position_z = pz;
			normal_x = normal_y = normal_z = 0;
			texcoord_x = texcoord_y = 0;
		}
		Vertex(float px, float py, float pz, float nx, float ny, float nz)
		{
			position_x = px;		position_y = py;		position_z = pz;
			normal_x = nx;		normal_y = ny;		normal_z = nz;
			texcoord_x = texcoord_y = 0;
		}
		Vertex(float px, float py, float pz, float tx, float ty)
		{
			position_x = px;		position_y = py;		position_z = pz;
			texcoord_x = tx;		texcoord_y = ty;
			normal_x = normal_y = normal_z = 0;
		}
		Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty)
		{
			position_x = px;		position_y = py;		position_z = pz;
			normal_x = nx;		normal_y = ny;		normal_z = nz;
			texcoord_x = tx;		texcoord_y = ty;
		}
		Vertex(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty,
			float ttx, float tty, float ttz, float bx, float by, float bz)
		{
			position_x = px;		position_y = py;		position_z = pz;
			normal_x = nx;		normal_y = ny;		normal_z = nz;
			texcoord_x = tx;		texcoord_y = ty;
			tangent_x = ttx;		tangent_y = tty; tangent_z = ttz;
			bitangent_x = bx; bitangent_y = by; bitangent_z = bz;
		}

		Vertex operator=(const Vertex &rhs)
		{
			position_x = rhs.position_x;	position_y = rhs.position_y;	position_z = rhs.position_z;
			normal_x = rhs.normal_x;		normal_y = rhs.normal_y;		normal_z = rhs.normal_z;
			texcoord_x = rhs.texcoord_x;	texcoord_y = rhs.texcoord_y;
			tangent_x = rhs.tangent_x; tangent_y = rhs.tangent_y; tangent_z = rhs.tangent_z;
			bitangent_x = rhs.bitangent_x; bitangent_y = rhs.bitangent_y; bitangent_z = rhs.bitangent_z;
			return (*this);
		}
	};
	enum PrimitiveType {
		/// A list of points, 1 vertex per point
		POINT_LIST = 1,
		/// A list of lines, 2 vertices per line
		LINE_LIST = 2,
		/// A strip of connected lines, 1 vertex per line plus 1 start vertex
		LINE_STRIP = 3,
		/// A list of triangles, 3 vertices per triangle
		TRIANGLE_LIST = 4,
		/// A strip of triangles, 3 vertices for the first triangle, and 1 per triangle after that 
		TRIANGLE_STRIP = 5,
		/// A fan of triangles, 3 vertices for the first triangle, and 1 per triangle after that
		TRIANGLE_FAN = 6
	};



	class RenderObject {
	public:
		typedef std::shared_ptr<RenderObject> ptr;
	public:
		RenderObject(): _type(TRIANGLE_LIST){}
		virtual ~RenderObject() = default;
	public:		
		void draw(Shader::ptr shader);
		void setVertices(std::vector<Vertex> vertices) { vertices = vertices; }
		void setIndex(std::vector<unsigned int> indices) { _indices = indices; }
		void setPrimitiveType(PrimitiveType type) { _type = type; }
	public:
		virtual void setPosition(glm::vec3 position){}
		
	protected:
		void createBuffer();
		virtual void setShaderUniform(Shader::ptr shader) = 0;

		PrimitiveType _type;
		uint32_t _vao, _vbo, _ebo;
		std::vector<Vertex> _vertices;
		std::vector<uint32_t> _indices;

	};
}

