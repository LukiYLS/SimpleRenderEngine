#pragma once

#include "Material.h"
#include "..\Core\Shader.h"
#include "..\Core\Uniform.h"
namespace SRE {

	class ShaderMaterial : public Material {

	public:

		void addUniform(Uniform::ptr uniform);
		void setShader(Shader::ptr shader);
		Shader::ptr getShader()const {}
		MaterialType getType()const { return _type; }
		void uploadUniforms();
	protected:

		Shader::ptr _shader;

		std::vector<Uniform::ptr> _uniforms;

		MaterialType _type;
	};
}