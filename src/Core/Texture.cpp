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

	void Texture::loadImage(Image* image)
	{
		if (!image)
			return;

		_width = image->width();
		_height = image->height();
		_depth = image->depth();

		_pixelFormat = image->pixelFormat();
		size_t maxMips = PixelUtil::getMaxMipmaps(_width, _height, _depth, _pixelFormat);
		if (_numMipMaps > maxMips)
			_numMipMaps = maxMips;

		size_t width = _width;
		size_t height = _height;
		size_t depth = _depth;
		for (unsigned short mip = 0; mip < _numMipMaps; mip++)
		{
			if (mip == 0)
			{
				_image_list.push_back((Image::ptr)image);
				if (_autoGenerateMipMap)
					break;
			}
			else
			{
				//_image_list.push_back(texture_manager_impl_->rescale_picture(image_information_, width, height));
			}				
			if (width > 1)
				width = width / 2;
			if (height > 1)
				height = height / 2;
			if (depth > 1)
				depth = depth / 2;
		}

		createInternalResources();


		size_t faces;
		bool multiImage; // Load from multiple images?
		if (_image_list.size() > 1)
		{
			faces = _image_list.size();
			multiImage = true;
		}
		else
		{
			faces = 1;//_image_list[0]->getNumFaces();
			multiImage = false;
		}
		for (size_t mip = 0; mip <= _numMipMaps; ++mip)
		{
			for (size_t i = 0; i < faces; ++i)
			{
				Image::ptr image = _image_list[i];
				PixelBox::ptr box = std::make_shared<PixelBox>(image->width(), image->height(), image->depth(), image->pixelFormat(), image->data());
				getBuffer(i, mip)->blitFromMemory(box);
			}
		}
	}
	void Texture::upLoad()
	{
		
		

	}
	void Texture::createInternalResources()
	{
		GLenum target = getTextureTarget();;
		glGenTextures(1, &_textureID);
		glBindTexture(target, _textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, _numMipMaps);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		unsigned width = _width;
		unsigned height = _height;
		unsigned depth = _depth;
		GLenum format = PixelUtil::getClosestGLInternalFormat(_pixelFormat);

		if (PixelUtil::isCompressed(_pixelFormat))
		{
		}
		else
		{
			for (unsigned short mip = 0; mip < _numMipMaps; mip++)
			{
				switch (_textureType)
				{
				case TEX_TYPE_1D:
					glTexImage1D(GL_TEXTURE_1D, mip, format,
						width, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, 0);

					break;
				case TEX_TYPE_2D:
					glTexImage2D(GL_TEXTURE_2D, mip, format,
						width, height, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, 0);
					break;
				case TEX_TYPE_2D_ARRAY:
				case TEX_TYPE_3D:
					glTexImage3D(target, mip, format,
						width, height, depth, 0,
						GL_RGBA, GL_UNSIGNED_BYTE, 0);
					break;
				case TEX_TYPE_CUBE_MAP:
					for (int face = 0; face<6; face++) {
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, format,
							width, height, 0,
							GL_RGBA, GL_UNSIGNED_BYTE, 0);
					}
					break;
				case TEX_TYPE_2D_RECT:
					break;
				};
				if (width>1)
					width = width / 2;
				if (height>1)
					height = height / 2;
				if (depth>1 && _textureType != TEX_TYPE_2D_ARRAY)
					depth = depth / 2;
			}
		}

		_surface_list.clear();

		for (GLuint face = 0; face < getNumFaces(); face++)
		{
			for (unsigned short mip = 0; mip < _numMipMaps; mip++)
			{
				HardwareTextureBuffer::ptr buffer = std::make_shared<HardwareTextureBuffer>(target, _textureID, face, mip, _usage, _autoGenerateMipMap);
				_surface_list.push_back(buffer);
			}
		}
	}
	HardwareTextureBuffer::ptr Texture::getBuffer(size_t face, size_t mipmap)
	{
		if (face > getNumFaces())
		{
			return NULL;
		}
		if (mipmap > _numMipMaps)
		{
			return NULL;
		}
		unsigned long idx = face*(_numMipMaps + 1) + mipmap;
		return idx < _surface_list.size() ? _surface_list[idx] : NULL;
		
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