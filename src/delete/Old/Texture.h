#ifndef TEXTURE_H
#define TEXTURE_H
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <memory>
#include "../Texture/Resource.h"

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
	class Texture 
		:public Resource{
	public:
		typedef std::shared_ptr<Texture> ptr;
	public:
		Texture();
		~Texture();
	public:
		virtual void setTextureType(TextureType type) { _type = type; }
		
		virtual TextureType getTextureType(void) const { return _type; }
		
		virtual uint8_t getNumMipmaps(void) const { return _mipmaps; }
		
		virtual void setNumMipmaps(uint8_t num) { _mipmaps = num; }

		virtual uint32_t getHeight() const { return _height; }
		virtual void setHeight(uint32_t height) { _height = height; }

		virtual uint32_t getWidth(void) const { return _width; }
		virtual void setWidth(uint32_t width) { _width = width; }

		virtual uint32_t getDepth(void) const { return _depth; }
		virtual void setDepth(uint32_t depth) { _depth = depth; }
	private:
		uint32_t _height;
		uint32_t _width;
		uint32_t _depth;
		TextureType _type;
		uint8_t _mipmaps;
		//GLuint _texID;

	};
}
#endif // !TEXTURE_H

