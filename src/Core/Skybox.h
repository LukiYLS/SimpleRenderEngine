#pragma once
#include <glew\glew.h>
#include <vector>
#include <string>
#include "Camera.h"
namespace Core {
	class Skybox
		:public Object{
	public:
		Skybox(std::vector<std::string> fileNames);
		~Skybox();
		void render(Camera* camera);
	private:
		GLuint _textureID;
	};
}