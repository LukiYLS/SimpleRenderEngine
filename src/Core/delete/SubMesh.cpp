#include "SubMesh.h"

namespace Core {

	void SubMesh::setShaderUniform(Shader::ptr shader)
	{
		shader->use();
		//lights
		for (auto it = _lights.begin(); it != _lights.end(); it++)
		{
			if ((*it)->getType() == DirectLight)
			{
				shader->setVec3("dirLight.direction", (*it)->getDirection());
				shader->setVec3("dirLight.ambient", (*it)->getAmbient());
				shader->setVec3("dirLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("dirLight.specular", (*it)->getSpecular());
			}
			else if ((*it)->getType() == PointLight)
			{
				shader->setVec3("pointLight.position", (*it)->getPosition());
				shader->setVec3("pointLight.ambient", (*it)->getAmbient());
				shader->setVec3("pointLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("pointLight.specular", (*it)->getSpecular());

				shader->setFloat("pointLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("pointLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("pointLight.quadratic", (*it)->getQuadraticAttenuation());
			}
			else if ((*it)->getType() == SpotLight)
			{
				shader->setVec3("spotLight.position", (*it)->getPosition());
				shader->setVec3("spotLight.direction", (*it)->getDirection());
				shader->setVec3("spotLight.ambient", (*it)->getAmbient());
				shader->setVec3("spotLight.diffuse", (*it)->getDiffuse());
				shader->setVec3("spotLight.specular", (*it)->getSpecular());

				shader->setFloat("spotLight.constant", (*it)->getConstantAttenuation());
				shader->setFloat("spotLight.linear", (*it)->getLinearAttenuation());
				shader->setFloat("spotLight.quadratic", (*it)->getQuadraticAttenuation());

				shader->setFloat("spotLight.innerCutoff", (*it)->getInnerCutoff());
				shader->setFloat("spotLight.outerCutoff", (*it)->getOuterCutoff());
			}
		}
		//material
		if (_hasMaterial)
		{
			shader->setVec3("ambientMt", _material.ambient);
			shader->setVec3("diffuseMt", _material.diffuse);
			shader->setVec3("specularMt", _material.specular);
			shader->setFloat("shininessMt", _material.shininess);
		}
		//texture
		if (!_textures.empty())
		{
			for (int i = 0; i < _textures.size(); i++)
			{
				TextureManager::Inst()->bindTexture(_textures[i], i);
				shader->setInt(_textures[i], i);
			}
		}
		//modelmatrix
		shader->setMat4("modelMatrix", glm::translate(glm::mat4(), _position));

	}
}