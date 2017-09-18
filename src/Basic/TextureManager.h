//**********************************************
//从freeimage移植过来
//**********************************************

#ifndef TextureManager_H
#define TextureManager_H

#include <windows.h>
#include <glew\glew.h>
#include "FreeImage.h"
#include <map>
#include <vector>
#include "Texture.h"
namespace Core {
	class TextureManager//这个一定要重构，所有的纹理怎么可能只用一个manager啊！怎么改？？
	{
	public:
		static TextureManager* Inst();
		virtual ~TextureManager();

		Texture::ptr createFromFile(const char* filename);
		bool loadTexture(const char* filename, const char* texName, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0);

		bool unloadTexture(const char* texName);

		bool loadCubeMap(std::vector<const char*> flies);

		bool bindTexture(const char* texName, int units);
		bool bindTexture(const char* texName, bool isCube);

		GLuint getTextureUnit(const char* texName);

		void unloadAllTextures();

		void addTexture(const char* texName, GLuint textureID);

	protected:
		TextureManager();//阻止构造
		TextureManager(const TextureManager& tm);//阻止拷贝
		TextureManager& operator=(const TextureManager& tm);//阻止赋值

		static TextureManager* m_inst;
		std::map<const char*, GLuint> m_texID;
		unsigned int m_cubeMap;
	};

}

#endif