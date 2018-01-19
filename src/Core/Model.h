#pragma once

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include "RenderObject.h"
namespace SRE {
	
	//mesh,submesh --> sub mesh¹²Ïímesh¶¥µã

	class Model : public Object{
	public:

		void loadModel(const std::string& path);
		void saveModel(const std::string fileName);
	protected:
		void processNode(aiNode* node, const aiScene* scene);
		aiScene* modelToAiScene();
		RenderObject* processMesh(aiMesh* mesh, const aiScene* scene);
		Material::ptr parseMaterial(aiMaterial *mat);
		TextureUnitState::ptr parseTexture(aiTextureType type, aiMaterial* aiMat);
		string directory;
		//std::vector<RenderObject::ptr> 

	};
}