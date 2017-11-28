#pragma once
#include "RenderObject.h"
#include "Shader.h"
#include "Uniform.h"
#include "RenderState.h"
#include "..\Material\Material.h"
#include <string>
#include <map>
namespace SRE {

	
	
	class Mesh
		: public RenderObject
	{
	public:
		typedef std::shared_ptr<Mesh> ptr;
	public:
		virtual Mesh::ptr asMesh() { return (ptr)this; }
		virtual const Mesh::ptr asMesh() const { return (ptr)this; }
		//void addTexture(const std::string& texName) { _textures.push_back(texName); }
		//void removeTexture(const std::string& texName) {}
		void setMaterial(Material::ptr material) { _material = material; }
		Material::ptr getMaterial() const { return _material; }
		void setShaderName(const std::string& name) { _shader_name = name; }
		const std::string& getShaderName() { return _shader_name; }
		void addUniform(Uniform* uniform) { _uniforms.push_back((Uniform::ptr)uniform); }
		void setRenderState(RenderState* renderstate) { _renderState = (RenderState::ptr)renderstate; }
		RenderState* getRenderState() { return _renderState.get(); }
		void setupUniform(Shader* shader);
		virtual void setShaderUniform(Shader* shader) {}
		bool getReceiveShadow()const { return _receiveShadow; }
		void setReceiveShadow(bool receive) { _receiveShadow = receive; }
	protected:		
		std::vector<std::string> _textures;
		Material::ptr _material;	
		std::string _shader_name;
		std::vector<Uniform::ptr> _uniforms;
		RenderState::ptr _renderState;
		bool _receiveShadow;
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
		::Vector3D position) { _position = position; }
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
		//Vector3D _position;		
		//bool _hasMaterial;
		//float _boundRadius;
		//std::map<const char*, Texture::ptr> _texture_map;
	private:
		unsigned int _vao, _vbo, _ebo;
	};*/
}


