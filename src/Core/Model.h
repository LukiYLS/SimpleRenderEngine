#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "RenderObject.h"
namespace SRE {
	
	//mesh,submesh --> sub mesh¹²Ïímesh¶¥µã

	class Model : public Object{
	public:

		void loadModel(const std::string& path);

	protected:
		void processNode(aiNode* node, const aiScene* scene);
		RenderObject* processMesh(aiMesh* mesh, const aiScene* scene);
		Material::ptr parseMaterial(aiMaterial *mat);
		TextureUnitState::ptr parseTexture(aiTextureType type, aiMaterial* aiMat);
		string directory;
		//std::vector<RenderObject::ptr> 

	};
}