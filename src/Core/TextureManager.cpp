
#include "TextureManager.h"

namespace Core {
	TextureManager* TextureManager::m_inst(0);

	TextureManager* TextureManager::Inst()
	{
		if (!m_inst)
			m_inst = new TextureManager();

		return m_inst;
	}

	TextureManager::TextureManager()
	{

#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
#endif
	}



	TextureManager::~TextureManager()
	{

#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif

		unloadAllTextures();
		m_inst = 0;
	}

	bool TextureManager::loadTexture(const char* fileName, const char* texName, GLenum image_format, GLint internal_format, GLint level, GLint border)
	{

		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);
		unsigned int width(0), height(0);
		GLuint gl_texID;

		fif = FreeImage_GetFileType(fileName, 0);

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(fileName);

		if (fif == FIF_UNKNOWN)
			return false;


		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, fileName);

		if (!dib)
			return false;
		dib = FreeImage_ConvertTo24Bits(dib);
		
		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
		
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);

		if ((pixels == 0) || (width == 0) || (height == 0))
			return false;



		if (m_texID.find(texName) != m_texID.end())
			glDeleteTextures(1, &(m_texID[texName]));


		glGenTextures(1, &gl_texID);

		m_texID[texName] = gl_texID;

		glBindTexture(GL_TEXTURE_2D, gl_texID);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
			border, image_format, GL_UNSIGNED_BYTE, pixels);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	bool TextureManager::loadCubeMap(std::vector<const char*> files)
	{
		glGenTextures(1, &m_cubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);
		unsigned int width(0), height(0);
		for (int i = 0; i < files.size(); i++)
		{
			const char* filename = files[i];
			fif = FreeImage_GetFileType(filename, 0);
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(filename);
			if (fif == FIF_UNKNOWN)
				return false;

			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, filename);
			if (!dib)
				return false;
			dib = FreeImage_ConvertTo24Bits(dib);
			BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			if ((pixels == 0) || (width == 0) || (height == 0))
				return false;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels
			);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool TextureManager::unloadTexture(const char* texName)
	{
		bool result(true);
		if (m_texID.find(texName) != m_texID.end())
		{
			glDeleteTextures(1, &(m_texID[texName]));
			m_texID.erase(texName);
		}
		else
		{
			result = false;
		}

		return result;
	}

	bool TextureManager::bindTexture(const char* texName,int units)
	{
		bool result(true);

		if (m_texID.find(texName) != m_texID.end())
		{
			glActiveTexture(GL_TEXTURE0 + units);
			glBindTexture(GL_TEXTURE_2D, m_texID[texName]);			
		}
		else
			result = false;

		return result;
	}
	unsigned int TextureManager::getTextureUnit(const char* texName)
	{
		if (m_texID.find(texName) != m_texID.end())
			return m_texID[texName];
		return 0;
	}

	void TextureManager::addTexture(const char* texName, GLuint textureID)
	{
		if (m_texID.find(texName) != m_texID.end())
			glDeleteTextures(1, &(m_texID[texName]));
		m_texID[texName] = textureID;
	}

	void TextureManager::unloadAllTextures()
	{
		std::map<const char*, GLuint>::iterator it = m_texID.begin();
		while (it != m_texID.end())
			unloadTexture(it->first);

		m_texID.clear();
	}
}
