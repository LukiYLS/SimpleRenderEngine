#ifndef TEXTURE_H
#define TEXTURE_H
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <memory>
namespace Basic {
	class Texture {//所有的纹理继承这个,后期会是一个单独的组件
	public:
		typedef std::shared_ptr<Texture> ptr;
	public:
		Texture();
		~Texture();
	public:
		bool loadTexture(const char* fileName);
		bool bindTexture();
	private:
		GLuint _texID;
	};
}
#endif // !TEXTURE_H

