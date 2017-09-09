#ifndef MESH_H
#define MESH_H
#include "Entity.h"
#include "Shader.h"
#include "Uniform.h"
#include "Light.h"
#include "Camera.h"
#include <vector>
#include <map>
//#include "glm\glm.hpp"
/*struct VertexFormat
{
	float position_x, position_y, position_z;
	float normal_x, normal_y, normal_z;
	float texcoord_x, texcoord_y;
	float tangent_x, tangent_y, tangent_z;
	float bitangent_x, bitangent_y, bitangent_z;
	VertexFormat()
	{
		position_x = position_y = position_z = 0;
		normal_x = normal_y = normal_z = 0;
		texcoord_x = texcoord_y = 0;
	}
	VertexFormat(float px, float py, float pz)
	{
		position_x = px;		position_y = py;		position_z = pz;
		normal_x = normal_y = normal_z = 0;
		texcoord_x = texcoord_y = 0;
	}
	VertexFormat(float px, float py, float pz, float nx, float ny, float nz)
	{
		position_x = px;		position_y = py;		position_z = pz;
		normal_x = nx;		normal_y = ny;		normal_z = nz;
		texcoord_x = texcoord_y = 0;
	}
	VertexFormat(float px, float py, float pz, float tx, float ty)
	{
		position_x = px;		position_y = py;		position_z = pz;
		texcoord_x = tx;		texcoord_y = ty;
		normal_x = normal_y = normal_z = 0;
	}
	VertexFormat(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty)
	{
		position_x = px;		position_y = py;		position_z = pz;
		normal_x = nx;		normal_y = ny;		normal_z = nz;
		texcoord_x = tx;		texcoord_y = ty;
	}
	VertexFormat(float px, float py, float pz, float nx, float ny, float nz, float tx, float ty,
		float ttx, float tty, float ttz, float bx, float by, float bz)
	{
		position_x = px;		position_y = py;		position_z = pz;
		normal_x = nx;		normal_y = ny;		normal_z = nz;
		texcoord_x = tx;		texcoord_y = ty;
		tangent_x = ttx;		tangent_y = tty; tangent_z = ttz;
		bitangent_x = bx; bitangent_y = by; bitangent_z = bz;
	}

	VertexFormat operator=(const VertexFormat &rhs)
	{
		position_x = rhs.position_x;	position_y = rhs.position_y;	position_z = rhs.position_z;
		normal_x = rhs.normal_x;		normal_y = rhs.normal_y;		normal_z = rhs.normal_z;
		texcoord_x = rhs.texcoord_x;	texcoord_y = rhs.texcoord_y;
		tangent_x = rhs.tangent_x; tangent_y = rhs.tangent_y; tangent_z = rhs.tangent_z;
		bitangent_x = rhs.bitangent_x; bitangent_y = rhs.bitangent_y; bitangent_z = rhs.bitangent_z;
		return (*this);
	}
};

//for assimp file
struct TextureR
{
	unsigned int id;
	std::string type;
	std::string texturePath;

};

class Shader;


class Mesh
{
public:

	Mesh();

	~Mesh();

	void destory(void);

	void setupMesh(void);

	void setupMesh(std::vector<VertexFormat>&, std::vector<unsigned int >&ic, std::vector<TextureR>&ts);

	void draw(const Shader*shader)const;

private:
	unsigned int                    _VAOID;

	unsigned int                    _VBOID;
	unsigned int                    _IBOID; //indices index

	unsigned int					_indicesCount;

	std::vector<TextureR> _textures;
	std::vector<VertexFormat> _vertexs;
	std::vector<unsigned int > _elementIndices;

};*/

using namespace Basic;
namespace Basic {
	struct Material//材质应该和纹理封装在一块吧？
	{
		glm::vec3 ambient;   // 环境光
		glm::vec3 diffuse;    // 漫反射光
		glm::vec3 specular;   // 镜面光
		float shininess; //镜面高光系数
	};
	class Mesh
		:public Entity {
	public:
		typedef std::shared_ptr<Mesh> ptr;
		Mesh();
		virtual ~Mesh() {}
		Mesh(const Mesh& mesh) {}
	public:	
		virtual void draw(RenderParams* params);
		//virtual void render(Shader::ptr shader);
		virtual void setVertices(std::vector<Vertex> vertices) { _vertices = vertices; }
		virtual void setIndex(std::vector<unsigned int> indices) { _indices = indices; }		
		virtual void createBuffer();
		virtual void setupLights(std::vector<Light::ptr> lights);
		virtual void setModelMatrix(glm::mat4x4& matrix) { _modelMatrix = matrix; }
		virtual void addTexture(const char* texName) { _textures.push_back(texName); }
		void setMaterial(Material material) { _material = material; }
		void setProgram(Shader::ptr shader);
		

		bool loadFromObjFile(const char* filename);
		//void addTexture(Texture::ptr texture);

	public:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<const char*> _textures;
		Shader::ptr _shader;
		Material _material;
		glm::mat4x4 _modelMatrix;

		bool _hasMaterial;

		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int _vao, _vbo, _ebo;
	};
}

#endif // !MESH_H

