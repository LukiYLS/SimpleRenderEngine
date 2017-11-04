#include "Texture.h"
#include <FreeImage.h>

namespace Core {

	Texture::Texture(GLenum textureTarget, const std::string& fileName)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);
		unsigned int width(0), height(0);
		GLuint gl_texID;

		fif = FreeImage_GetFileType(fileName.c_str(), 0);

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(fileName.c_str());

		if (fif == FIF_UNKNOWN)
			return;


		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, fileName.c_str());

		if (!dib)
			return;
		dib = FreeImage_ConvertTo24Bits(dib);

		BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

		width = FreeImage_GetWidth(dib);
		height = FreeImage_GetHeight(dib);

		if ((pixels == 0) || (width == 0) || (height == 0))
			return;

		glGenTextures(1, &_textureID);
		glBindTexture(textureTarget, _textureID);

		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(textureTarget, 0, GL_RGBA, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glBindTexture(textureTarget, 0);
		_textureTarget = textureTarget;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_textureID);
		glDeleteSamplers(1, &_sampler);
	}

	void Texture::setFiltering(int magnification, int minification)
	{
		glBindSampler(0, _sampler);
		
		if (magnification == TEXTURE_FILTER_MAG_NEAREST)
			glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		else if (magnification == TEXTURE_FILTER_MAG_BILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		if (minification == TEXTURE_FILTER_MIN_NEAREST)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_BILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else if (minification == TEXTURE_FILTER_MIN_NEAREST_MIPMAP)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_BILINEAR_MIPMAP)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else if (minification == TEXTURE_FILTER_MIN_TRILINEAR)
			glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	void Texture::bindTexture(int unit)
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(_textureTarget, _textureID);		
		glBindSampler(unit, _sampler);
	}

}