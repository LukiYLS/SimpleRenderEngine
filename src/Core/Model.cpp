#include "Model.h"
#include "TextureManager.h"
//#include "../Thread/Thread.h"
#include <assimp/postprocess.h>
namespace SRE {

	Object* Model::loadModel(const std::string& path)
	{

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // 
		{
			//cout << "Failed";
			return NULL;
		}
		directory = path.substr(0, path.find_last_of('/'));

		return processNode(scene->mRootNode, scene);
	}

	void Model::saveModel(const std::string fileName)
	{
		Assimp::Exporter exporter;
		//aiScene* scene = modelToAiScene(this);
		//exporter.Export((scene,)
	}

	Object* Model::processNode(aiNode* node, const aiScene* scene)
	{		
		aiMatrix4x4 transfrom = node->mTransformation;
		aiVector3D position, scale;
		aiQuaternion rotation;
		transfrom.Decompose(scale, rotation, position);
		Object* object = new Object;
		object->setPosition(Vector3D(position.x, position.y, position.z));
		object->setScale(Vector3D(scale.x, scale.y, scale.z));
		object->setOrientation(Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{			
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			object->add(processMesh(mesh, scene));
		}
	
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			object->add(processNode(node->mChildren[i], scene));
		}
		return object;
	}

	RenderObject* Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		RenderObject* renderObject = new RenderObject;

		if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT)
			renderObject->setPrimitiveType(POINT_LIST);
		else if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE)
			renderObject->setPrimitiveType(LINE_LIST);
		else if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)
			renderObject->setPrimitiveType(TRIANGLE_LIST);
		else
			renderObject->setPrimitiveType (TRIANGLE_LIST);
		std::vector<Vertex> vertices;
		BoundingBox bbox;
		BoundingSphere sphere;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;

			vertex.position_x = mesh->mVertices[i].x;
			vertex.position_y = mesh->mVertices[i].y;
			vertex.position_z = mesh->mVertices[i].z;

			bbox.expandByPoint(Vector3D(vertex.position_x, vertex.position_y, vertex.position_z));
			sphere.expandByPoint(Vector3D(vertex.position_x, vertex.position_y, vertex.position_z));

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
		renderObject->setBoundBox(bbox);
		renderObject->setBoundSphere(sphere);
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//解析material
		renderObject->setMaterial(parseMaterial(material));

		return renderObject;
		
		
	}

	Material::ptr Model::parseMaterial(aiMaterial *aiMat)
	{
		//是否有光照？
		Material* material = new Material;
	
		//////////////////////////////////////////////////////////////////////////
		aiColor4D color;
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		{
			material->setColor(Vector3D(color.r, color.g, color.b));		
		}

		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color))
		{
			material->setSpecular(Vector3D(color.r, color.g, color.b));
		}

		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, color))
		{
			material->setEmissive(Vector3D(color.r, color.g, color.b));
		}
		bool bvalue;
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, bvalue))
		{
			material->setTransparent(bvalue);
		}
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_ENABLE_WIREFRAME, bvalue))
		{
			material->setWireframe(bvalue);
		}
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_TWOSIDED, bvalue))
		{
			material->setCullFaceMode(CullFaceMode::DoubleSide);
		}
		float fvalue;
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_OPACITY, fvalue))
		{
			material->setOpacity(fvalue);
		}
		if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS, fvalue))
		{
			material->setShininess(fvalue);
		}
		//////////////////////////////////////////////////////////////////////////
		//texture parse
		parseTexture(aiTextureType_DIFFUSE, aiMat);
		return Material::ptr(material);

	}


	TextureUnitState::ptr Model::parseTexture(aiTextureType type, aiMaterial* aiMat)
	{
		aiString path;
		aiTextureMapping mapping;
		unsigned int uvindex;
		//PN_stdfloat blend;
		float blend;
		aiTextureOp op;
		int count = aiMat->GetTextureCount(type);//??????
		//aiMat->GetTexture()
		TextureUnitState* texture_unit = new TextureUnitState;
		for (int i = 0; i < count; i++)
		{
			if (AI_SUCCESS == aiMat->GetTexture(type, i, &path, &mapping, NULL, &blend, &op, NULL))
			{
				Texture::ptr texture = TextureManager::Inst()->loadTexture(path.C_Str(), (directory + "\\" + path.C_Str()).c_str());

				if (type == aiTextureType_OPACITY)
				{

				}
				switch (mapping)
				{
				case aiTextureMapping_UV:
					//2d
					break;
				case aiTextureMapping_BOX:
					//3d
					break;
				default:
					//2d
					break;
				}

				int wrapping;
				if (AI_SUCCESS == aiMat->Get(AI_MATKEY_MAPPINGMODE_U(type, 0), wrapping))
				{
					switch (wrapping)
					{
					case aiTextureMapMode_Wrap:
						texture_unit->setTextureAddressingMode(TAM_WRAP);
						break;
					case aiTextureMapMode_Clamp:
						texture_unit->setTextureAddressingMode(TAM_CLAMP);
						break;
					case aiTextureMapMode_Mirror:
						texture_unit->setTextureAddressingMode(TAM_MIRROR);
						break;
					}
				}
			}
		}
		

		return TextureUnitState::ptr(texture_unit);
	}
}