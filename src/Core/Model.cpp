#include "Model.h"
#include <assimp/postprocess.h>
namespace SRE {

	void Model::loadModel(const std::string& path)
	{

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 
		{
			//cout << "Failed";
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{		
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{			
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			add(processMesh(mesh, scene));
		}
	
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	RenderObject* Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		RenderObject* renderObject = new RenderObject;
		std::vector<Vertex> vertices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position_x = mesh->mVertices[i].x;
			vertex.position_y = mesh->mVertices[i].y;
			vertex.position_z = mesh->mVertices[i].z;

			vertex.normal_x = mesh->mNormals[i].x;
			vertex.normal_y = mesh->mNormals[i].y;
			vertex.normal_z = mesh->mNormals[i].z;

			if (mesh->mTextureCoords[0]) 
			{
				vertex.texcoord_x = mesh->mTextureCoords[0][i].x;
				vertex.texcoord_y = mesh->mTextureCoords[0][i].y;				
			}

			vertex.tangent_x = mesh->mTangents[i].x;
			vertex.tangent_y = mesh->mTangents[i].y;
			vertex.tangent_z = mesh->mTangents[i].z;
			
			vertex.bitangent_x = mesh->mBitangents[i].x;
			vertex.bitangent_y = mesh->mBitangents[i].y;
			vertex.bitangent_z = mesh->mBitangents[i].z;

			vertices.push_back(vertex);
		
		}
		std::vector<unsigned int> indices;
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		renderObject->setVertexData(vertices);
		renderObject->setIndexData(indices);

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//½âÎömaterial
		
	}
}