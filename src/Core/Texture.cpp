#include "Texture.h"
#include <FreeImage.h>
#include "HardwareBuffer\HardwareTextureBuffer.h"
#include "HardwareBuffer\PixelUtil.h"
namespace Core {


	Texture::Texture(const std::string name, TextureType type = TEX_TYPE_2D)
		:_name(name),
		_width(0),
		_height(0),
		_depth(0),
		_isAlpha(false)
	{

	}
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
		//_textureTarget = textureTarget;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_textureID);
		glDeleteSamplers(1, &_sampler);
	}
	void Texture::upLoad()
	{
		GLenum target = getTextureTarget();;
		
		glGenTextures(1, &_textureID);
		glBindTexture(target, _textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, _numMipMaps);

		GLenum format = PixelUtil::getGLInternalFormat(_pixelFormat);
		GLenum data_format = PixelUtil::getGLOriginFormat(_pixelFormat);
		GLenum data_type = PixelUtil::getGLOriginDataType(_pixelFormat);

		if (_autoGenerateMipMap)
		{
			//如果是cubemap，目前imagelist存储6张纹理（当_autoGenerateMipMap为false，这种情况？？）
			if (_textureType == TEX_TYPE_CUBE_MAP)
			{
				for (int i = 0; i < 6; ++i)
				{
					Image::ptr image = _image_list[i];
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
						0, GL_RGB, image->width(), image->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->data());

					HardwareTextureBuffer::ptr buffer = std::make_shared<HardwareTextureBuffer>(
						GL_TEXTURE_CUBE_MAP, _textureID, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, _usage, false);
					_surface_list.push_back(buffer);
					PixelBox::ptr box = std::make_shared<PixelBox>(_image_list[i]->width(),_image_list[i]->height(), _image_list[i]->depth(),_image_list[i]-)
					buffer->blitFromMemory()
				}
			}
			else
			{
				Image::ptr image = _image_list[0];

			}
			glGenerateMipmap(target);
		}
		for (size_t mip = 0; mip < _mipmaps; mip++)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, _mipmaps);
			//glTexImage2D(GL_TEXTURE_2D, mip, format, width, height, 0, data_format, data_type, 0);
		}

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
		//glBindTexture(_textureTarget, _textureID);		
		glBindSampler(unit, _sampler);
	}
	GLenum Texture::getTextureTarget()const
	{
		switch (_textureType)
		{
		case TEX_TYPE_1D:
			return GL_TEXTURE_1D;
		case TEX_TYPE_2D:
			return GL_TEXTURE_2D;
		case TEX_TYPE_3D:
			return GL_TEXTURE_3D;
		case TEX_TYPE_CUBE_MAP:
			return GL_TEXTURE_CUBE_MAP;
		case TEX_TYPE_2D_ARRAY:
			return GL_TEXTURE_2D_ARRAY_EXT;
		default:
			return 0;
		};
	}

}