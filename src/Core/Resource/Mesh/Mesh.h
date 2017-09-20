#ifndef MESH_H
#define MESH_H
#include "RenderObject.h"
#include "../Shader.h"
#include "../Light.h"
#include <vector>
#include <map>
namespace Core {
	struct Material//
	{
		glm::vec3 ambient;   // 环境光
		glm::vec3 diffuse;    // 漫反射光
		glm::vec3 specular;   // 镜面光
		float shininess; //镜面高光系数
	};
	class Mesh
		:public RenderObject {
	public:
		typedef std::shared_ptr<Mesh> ptr;
		Mesh();
		virtual ~Mesh() {}
		Mesh(const Mesh& mesh) {}
	public:			
		void setLights(std::vector<Light::ptr> lights) { _lights = lights; }
		void setPostion(glm::vec3 position) { _position = position; }
		void addTexture(const char* texName) { _textures.push_back(texName); }			
		void setMaterial(Material material) { _material = material; }
		
		void calculateBounding();
		

	protected:
		virtual void setShaderUniform(Shader::ptr shader);

	public:
		std::vector<Vertex> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<const char*> _textures;
		std::vector<Light::ptr> _lights;
		Material _material;	
		glm::vec3 _position;		
		bool _hasMaterial;
		float _boundRadius;
		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int _vao, _vbo, _ebo;
	};
}

#endif // !MESH_H

