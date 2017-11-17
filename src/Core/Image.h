#pragma once
#include <memory>
#include <string>
#include <FreeImage.h>
namespace Core {

	class Image {
	public:
		typedef std::shared_ptr<Image> ptr;
	public:
		Image();
		Image(const Image& image);
		virtual ~Image();
		Image & operator = (const Image & img);

	public:

		bool load(const std::string& file_name);
		bool save(const std::string& file_name);

		unsigned char* data() { return _buffer; }
		const unsigned char * data() const { return _buffer; }
		size_t size() const { return _bufSize; }
		
		unsigned char numMipmaps() const { return _numMipMaps; }

		unsigned int width(void) const { return _width; }
		
		unsigned int height(void) const { return _height; }
		
		unsigned int depth(void) const { return _depth; }
		
		size_t numFaces(void) const { return 1; }
		
		size_t rowSpan(void) const { return _width*_pixelSize; }
	
		unsigned char BPP() const { return _pixelSize * 8; }
	
		//bool getHasAlpha() const;		
	
		//ColourValue getColourAt(size_t x, size_t y, size_t z) const;
		
		//void setColourAt(ColourValue const &cv, size_t x, size_t y, size_t z);
		
		//PixelBox getPixelBox(size_t face = 0, size_t mipmap = 0) const;
		
		void freeMemory();
		void resize(int width, int height);
	protected:		
		bool decode(FIBITMAP* dib);
		FIBITMAP* rescale(FIBITMAP* dib, int width, int height);
		FIBITMAP* toBitMap();
		unsigned int _width;		
		unsigned int _height;	
		unsigned int _depth;	
		size_t _bufSize;		
		unsigned char _numMipMaps;
		unsigned char _pixelSize;
		unsigned char* _buffer;
		FREE_IMAGE_FORMAT _format;
		//std::string _type;

	};
}