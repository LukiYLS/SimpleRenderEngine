#ifndef MESH_H
#define MESH_H
#include "RenderObject.h"
#include "Shader.h"
#include "Uniform.h"
#include "Light.h"
#include "Group.h"
#include <vector>
#include <string>
#include <map>
namespace Core {
	
	class Mesh
		:public Group{
	public:
		typedef std::shared_ptr<Mesh> ptr;
		Mesh();
		virtual ~Mesh() {}
		Mesh(const Mesh& mesh) {}
	public:			
		virtual Mesh* asMesh() { return this; }
		virtual const Mesh* asMesh() const { return this; }

		//void setLights(std::vector<Light::ptr> lights) { _lights = lights; }
		//void setPostion(glm::vec3 position) { _position = position; }
		//void addTexture(const char* texName) { _textures.push_back(texName); }			
		//void setMaterial(Material material) { _material = material; }
		void addSubMesh(SubMesh::ptr submesh) { addChild(submesh); }
		void calculateBounding();
		

	protected:
		

	public:

		//std::vector<const char*> _textures;
		//std::map<std::string, std::string> _shader_submesh;
		//std::vector<Light::ptr> _lights;
		//Material _material;	
		//glm::vec3 _position;		
		//bool _hasMaterial;
		//float _boundRadius;
		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int _vao, _vbo, _ebo;
	};
}

#endif // !MESH_H

