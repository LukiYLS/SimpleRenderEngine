//**********************************************
//´ÓfreeimageÒÆÖ²¹ýÀ´
//**********************************************

#pragma once
#include <glew\glew.h>
#include <windows.h>
#include "FreeImage.h"
#include <map>
#include <vector>
#include "Texture.h"
namespace Core {
	class TextureManager
	{
	public:
		static TextureManager* Inst();
		virtual ~TextureManager();
	
		Texture::ptr loadTexture(const std::string& tex_name, const std::string& file_name, TextureType type = TEX_TYPE_2D);
		Texture::ptr loadCubeMap(const std::string& tex_name, std::vector<const std::string> flies);
		Texture::ptr getTexture(const std::string& tex_name);
		//bool loadTexture(const char* filename, const std::string texName, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0);
		//bool loadCubeMap(std::vector<const char*> flies);
		bool unloadTexture(std::string texName);		

		bool bindTexture(const std::string texName, int units);		

		

		void unloadAllTextures();

		void addTexture(const std::string texName, GLuint textureID);

	protected:
		TextureManager();//×èÖ¹¹¹Ôì
		TextureManager(const TextureManager& tm);//×èÖ¹¿½±´
		TextureManager& operator=(const TextureManager& tm);//×èÖ¹¸³Öµ

		static TextureManager* m_inst;
		std::map<std::string, GLuint> m_texID;
		typedef std::map<std::string, Texture::ptr> TextureMap;
		TextureMap _textures;
		unsigned int m_cubeMap;
	};

}

