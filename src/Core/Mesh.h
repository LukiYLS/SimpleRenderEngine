#pragma once
#include "RenderObject.h"
#include "Shader.h"
#include "Uniform.h"
#include "RenderState.h"
#include <string>
#include <map>
namespace Core {

	
	struct Material//
	{
		typedef std::shared_ptr<Material> ptr;
		Vector3D ambient;   // 环境光
		Vector3D diffuse;    // 漫反射光
		Vector3D specular;   // 镜面光
		float shininess; //镜面高光系数
	};
	class Mesh
		: public RenderObject
	{
	public:
		typedef std::shared_ptr<Mesh> ptr;
	public:
		virtual Mesh* asMesh() { return this; }
		virtual const Mesh* asMesh() const { return this; }
		void addTexture(const std::string& texName) { _textures.push_back(texName); }
		void removeTexture(const std::string& texName) {}
		void setMaterial(Material* material) { _material = (Material::ptr)material; }
		Material* getMaterial() const { return _material.get(); }
		void setShaderName(const std::string& name) { _shader_name = name; }
		const std::string& getShaderName() { return _shader_name; }
		void addUniform(Uniform* uniform) { _uniforms.push_back((Uniform::ptr)uniform); }
		void setRenderState(RenderState* renderstate) { _renderState = (RenderState::ptr)renderstate; }
		RenderState* getRenderState() { return _renderState.get(); }
		void setupUniform(Shader* shader);
	protected:		
		std::vector<std::string> _textures;
		Material::ptr _material;	
		std::string _shader_name;
		std::vector<Uniform::ptr> _uniforms;
		RenderState::ptr _renderState;
	};
	/*class Mesh
		:public Group{
	public:
		typedef std::shared_ptr<Mesh> ptr;
		Mesh();
		virtual ~Mesh() {}
		Mesh(const Mesh& mesh) {}
	public:			
		virtual Mesh* asMesh() { return this; }
		virtual const Mesh* asMesh() const { return this; }
		virtual void addSubMesh(SubMesh* sub);
		virtual void removeSubMesh(SubMesh* sub);
		virtual void removeSubMeshs(unsigned int pos, unsigned int num);
		virtual void setSubMesh(unsigned int i, SubMesh * sub);

		//void setLights(std::vector<Light::ptr> lights) { _lights = lights; }
		//void setPostion(
		::vec3 position) { _position = position; }
		//void addTexture(const char* texName) { _textures.push_back(texName); }			
		//void setMaterial(Material material) { _material = material; }
		SubMesh::ptr createSubMesh();
		void addSubMesh(SubMesh* submesh) { addChild(submesh); }
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
	};*/
}


