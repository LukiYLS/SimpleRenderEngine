//**********************************************
//´ÓfreeimageÒÆÖ²¹ıÀ´
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
	
		Texture::ptr createTexture(TextureType type, const std::string& name);

		bool loadTexture(const char* filename, const std::string texName, GLenum image_format = GL_RGB, GLint internal_format = GL_RGB, GLint level = 0, GLint border = 0);

		bool unloadTexture(std::string texName);

		bool loadCubeMap(std::vector<const char*> flies);

		bool bindTexture(const std::string texName, int units);		

		GLuint getTextureUnit(const std::string texName);

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

