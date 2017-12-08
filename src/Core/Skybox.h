#pragma once
#include <glew\glew.h>
#include <vector>
#include <string>
#include "Camera.h"
namespace SRE {
	class Skybox
		:public Object{
	public:
		typedef std::shared_ptr<Skybox> ptr;
		Skybox(std::vector<std::string> fileNames);
		~Skybox();
		void render(Camera* camera);
	private:
		GLuint _textureID;
	};
}