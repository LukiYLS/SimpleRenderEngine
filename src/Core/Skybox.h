#pragma once
#include <glew\glew.h>
#include <vector>
#include <string>
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
namespace SRE {
	class Skybox
		:public Object{
	public:
		typedef std::shared_ptr<Skybox> ptr;
		Skybox();
		Skybox(std::vector<std::string> fileNames);
		void setTexture(Texture::ptr texture) { _cubeMap = texture; }
		Texture::ptr getTexture()const { return _cubeMap; }
		~Skybox();
		void render(Camera* camera);
	private:
		Texture::ptr _cubeMap;
		Shader::ptr _skyboxShader;
		Mesh::ptr _skybox;
	};
}