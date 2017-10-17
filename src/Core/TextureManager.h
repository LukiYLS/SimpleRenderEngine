//**********************************************
//´ÓfreeimageÒÆÖ²¹ıÀ´
//**********************************************

#ifndef TextureManager_H
#define TextureManager_H
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
		TextureManager();//×èÖ¹¹¹Ôì
		TextureManager(const TextureManager& tm);//×èÖ¹¿½±´
		TextureManager& operator=(const TextureManager& tm);//×èÖ¹¸³Öµ

		static TextureManager* m_inst;
		std::map<const char*, GLuint> m_texID;
		unsigned int m_cubeMap;
	};

}

#endif