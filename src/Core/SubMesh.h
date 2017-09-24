#pragma once
#include "RenderObject.h"
#include "Node.h"
namespace Core {

	struct Material//
	{
		glm::vec3 ambient;   // 环境光
		glm::vec3 diffuse;    // 漫反射光
		glm::vec3 specular;   // 镜面光
		float shininess; //镜面高光系数
	};
	
	class SubMesh
		:public RenderObject, public Node{

	public:
		virtual SubMesh* asSubMesh() { return this; }
		virtual const SubMesh* asSubMesh()const { return this; }

		void addTexture(const std::string& texName) { _textures.push_back(texName); }
		void removeTexture(const std::string& texName){}
		void setMaterial(Material material) { _material = material; }
		Material const getMaterial() const { return _material; }
		void updateMatrixWorld();
	protected:
		friend class Mesh;
		std::vector<std::string> _textures;
		Material _material;
		Mesh* _parent;
	};
}