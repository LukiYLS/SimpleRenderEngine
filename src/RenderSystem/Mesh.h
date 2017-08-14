#ifndef MESH_H
#define MESH_H
#include "Entity.h"
#include "Shader.h"
#include <vector>
#include <map>
#include <glm\glm.hpp>

using namespace RenderSystem;
namespace RenderSystem {
	struct Material
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
		Mesh() {}
		virtual ~Mesh() {}
		Mesh(const Mesh& mesh) {}
	public:
		virtual void draw();
		virtual void setVertices(std::vector<Vertex> vertices) { _vertices = vertices; }
		virtual void setIndex(std::vector<unsigned int> indices) { _indices = indices; }		
		virtual void setupBuffer();
		virtual void setProgram(Shader::ptr shader);
		virtual void addTexture(const char* texName) { _textures.push_back(texName); }
		virtual void setMaterial(Material material) { _material = material; }
		virtual void setModelMatrix(glm::mat4 matrix) { _modelMatrix = matrix; }

		bool loadFromObjFile(const char* filename);
		//void addTexture(Texture::ptr texture);

	public:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<const char*> _textures;
		Shader::ptr _shader;
		Material _material;
		glm::mat4 _modelMatrix;

		bool _hasMaterial;

		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int _vao, _vbo, _ebo;
	};
}

#endif // !MESH_H

