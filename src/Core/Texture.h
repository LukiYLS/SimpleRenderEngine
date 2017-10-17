#pragma once
#include <glew\glew.h>
#include <memory>

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
		Texture();
		~Texture();
	public:
		void createEmpty2DTexture(int width, int height, GLenum fromat);
		void createEmpty3DTexture(int width, int height, int detph, GLenum fromat);
		void createEmpty2DTextureArray(int width, int height, int detph, GLenum fromat);
		
		virtual void setTextureType(TextureType type) { _type = type; }		
		virtual TextureType getTextureType(void) const { return _type; }
		
		virtual bool getNumMipmaps(void) const { return _mipmaps; }		
		virtual void setNumMipmaps(bool num) { _mipmaps = num; }

		void setFiltering(int magnification, int minification);	

		void bindTexture(int unit);
		void deleteTexture();

		virtual uint32_t getHeight() const { return _height; }
		virtual void setHeight(uint32_t height) { _height = height; }

		virtual uint32_t getWidth(void) const { return _width; }
		virtual void setWidth(uint32_t width) { _width = width; }

		virtual uint32_t getDepth(void) const { return _depth; }
		virtual void setDepth(uint32_t depth) { _depth = depth; }
	private:
		int _height, _width, _depth;
		GLuint _textureID; // Texture name
		GLuint _sampler; // Sampler name
		TextureType _type;
		bool _mipmaps;
	};
}


