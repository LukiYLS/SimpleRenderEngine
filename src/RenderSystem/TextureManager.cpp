
#include "TextureManager.h"

namespace RenderSystem {
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

		UnloadAllTextures();
		m_inst = 0;
	}

	bool TextureManager::LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib(0);
		//pointer to the image data
		//unsigned char* bits(0);
		//image width and height
		unsigned int width(0), height(0);
		//OpenGL's image ID to map to
		GLuint gl_texID;

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filename, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filename);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return false;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filename);
		//if the image failed to load, return failure
		if (!dib)
			return false;
		dib = FreeImage_ConvertTo24Bits(dib);
		//retrieve the image data
		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
		//get the image width and height
		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);
		//if this somehow one of these failed (they shouldn't), return failure
		if ((pixels == 0) || (width == 0) || (height == 0))
			return false;
		

		//if this texture ID is in use, unload the current texture
		if (m_texID.find(texID) != m_texID.end())
			glDeleteTextures(1, &(m_texID[texID]));

		//generate an OpenGL texture ID for this texture
		glGenTextures(1, &gl_texID);
		//store the texture ID mapping
		m_texID[texID] = gl_texID;
		//bind to the new texture ID
		glBindTexture(GL_TEXTURE_2D, gl_texID);
		//store the texture data for OpenGL use
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
			border, image_format, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		//Free FreeImage's copy of the data
		//FreeImage_Unload(dib);

		//return success
		return true;
	}

	bool TextureManager::UnloadTexture(const unsigned int texID)
	{
		bool result(true);
		//if this texture ID mapped, unload it's texture, and remove it from the map
		if (m_texID.find(texID) != m_texID.end())
		{
			glDeleteTextures(1, &(m_texID[texID]));
			m_texID.erase(texID);
		}
		//otherwise, unload failed
		else
		{
			result = false;
		}

		return result;
	}

	bool TextureManager::BindTexture(const unsigned int texID)
	{
		bool result(true);
		//if this texture ID mapped, bind it's texture as current
		if (m_texID.find(texID) != m_texID.end())
		{
			glActiveTexture(m_texID[texID]);	
			glBindTexture(GL_TEXTURE_2D, m_texID[texID]);
		}
			
		//otherwise, binding failed
		else
			result = false;

		return result;
	}
	unsigned int TextureManager::getTextureID(int index)
	{
		if (m_texID.find(index) != m_texID.end())
			return m_texID[index];
		return 0;
	}

	void TextureManager::UnloadAllTextures()
	{
		//start at the begginning of the texture map
		std::map<unsigned int, GLuint>::iterator i = m_texID.begin();

		//Unload the textures untill the end of the texture map is found
		while (i != m_texID.end())
			UnloadTexture(i->first);

		//clear the texture map
		m_texID.clear();
	}
}
