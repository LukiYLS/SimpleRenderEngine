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

