#include "Mesh.h"
#include "TextureManager.h"
#include "../Math/Vector2D.h"
namespace Core {

	void Mesh::setupUniform(Shader* shader)
	{
		shader->use();
		//_uniforms
		for (auto uniform : _uniforms)
		{
			switch (uniform->getType())
			{
			case FLOAT:
			{
				float value = uniform->getValue();
				shader->setFloat(uniform->getName(), value);
				break;
			}				
			case FLOAT_VEC2:
			{
				Vector2D value = uniform->getValue();
				shader->setVec2(uniform->getName(), value);
				break;
			}				
			case FLOAT_VEC3:
			{
				Vector3D value = uniform->getValue();
				shader->setVec3(uniform->getName(), value);
				break;
			}				
			case FLOAT_VEC4:
			{
				Vector4D value = uniform->getValue();
				shader->setVec4(uniform->getName(), value);
				break;
			}				
			default:
				break;
			}
		}
		//material
		if (_material)
		{
			shader->setVec3("ambientMt", _material->ambient);
			shader->setVec3("diffuseMt", _material->diffuse);
			shader->setVec3("specularMt", _material->specular);
			shader->setFloat("shininessMt", _material->shininess);
		}
		//texture
		if (!_textures.empty())
		{
			for (int i = 0; i < _textures.size(); i++)
			{
				TextureManager::Inst()->bindTexture(_textures[i].c_str(), i);
				shader->setInt(_textures[i].c_str(), i);
			}
		}
		
	}
	
}