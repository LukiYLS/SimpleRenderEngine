#pragma once
#include <glew\glew.h>
#include <memory>
#include <string>
#include <vector>
#include "Image.h"
namespace Core {
	enum TextureType
	{	
		TEX_TYPE_1D = 1,	
		TEX_TYPE_2D = 2,	
		TEX_TYPE_3D = 3,	
		TEX_TYPE_CUBE_MAP = 4,	
		TEX_TYPE_2D_ARRAY = 5,	
		TEX_TYPE_2D_RECT = 6
	};
	enum TextureFiltering
	{
		TEXTURE_FILTER_MAG_NEAREST = 0,
		TEXTURE_FILTER_MAG_BILINEAR,
		TEXTURE_FILTER_MIN_NEAREST, 
		TEXTURE_FILTER_MIN_BILINEAR, 
		TEXTURE_FILTER_MIN_NEAREST_MIPMAP, 
		TEXTURE_FILTER_MIN_BILINEAR_MIPMAP,
		TEXTURE_FILTER_MIN_TRILINEAR, 
	};
	class Texture 
		{
	public:
		typedef std::shared_ptr<Texture> ptr;
	public:
		Texture(TextureType type) {}
		Texture(GLenum textureTarget, const std::string& fileName);
		~Texture();
	public:		
		//virtual void setTextureType(TextureType type) { _type = type; }		
		//virtual TextureType getTextureType(void) const { return _type; }
		void upLoad();
		virtual bool getNumMipmaps(void) const { return _mipmaps; }		
		virtual void setNumMipmaps(bool num) { _mipmaps = num; }
		void setFiltering(int magnification, int minification);	
		void bindTexture(int unit);
		
		void setTextureType(TextureType type) { _textureType = type; }
		TextureType getTextureType(void) const { return _textureType; }
		size_t getNumFaces()const { return _textureType == TEX_TYPE_CUBE_MAP ? 6 : 1; }

		void loadImage(Image* image);
		//virtual uint32_t getHeight() const { return _height; }
		//virtual void setHeight(uint32_t height) { _height = height; }
		//virtual uint32_t getWidth(void) const { return _width; }
		//virtual void setWidth(uint32_t width) { _width = width; }
		//virtual uint32_t getDepth(void) const { return _depth; }
		//virtual void setDepth(uint32_t depth) { _depth = depth; }
	protected:
		GLenum getTextureTarget()const;
		std::vector<Image::ptr> _image_list;
		//int _height, _width, _depth;
		GLuint _textureID; // Texture name
		GLuint _sampler; // Sampler name
		//GLenum _textureTarget;
		unsigned int _mipmaps;
		bool _autoGenerateMipMap;
		TextureType _textureType;
	};
}


