#include "Image.h"


namespace Core {

	Image::Image()
		: _width(0),
		  _height(0),
		  _depth(0),
		 _bufSize(0),
		_numMipMaps(0),
		_format(),
		_buffer(NULL)
	{
		FreeImage_Initialise();
	}
	Image::~Image()
	{
		if (_buffer)
		{
			free(_buffer);
			_buffer = 0;
		}
		FreeImage_DeInitialise();
	}
	bool Image::load(const std::string& file_name)
	{
		FREE_IMAGE_FORMAT  fif = FIF_UNKNOWN;
		fif = FreeImage_GetFIFFromFilename(file_name.c_str());

		if (fif == FIF_UNKNOWN)
		{
			return false;
		}

		FIBITMAP * dib;
		if (FreeImage_FIFSupportsReading(fif))
		{
			dib = FreeImage_Load(fif, file_name.c_str());
		}
		else
		{
			return false;
		}

		return decode(dib);
	}
	bool Image::save(const std::string& file_name)
	{
		if (!_buffer)
			return false;

		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(file_name.c_str());
		
		FIBITMAP* dib = toBitMap();
		bool success = false;
		if (FreeImage_Save(fif, dib, file_name.c_str(), 0))
		{
			success = true;
		}
		else
		{
			success = false;
		}
		FreeImage_Unload(dib);

		return success;
	}
	bool Image::decode(FIBITMAP* dib)
	{
		if (!dib)
		{
			return false;
		}

		unsigned int width = FreeImage_GetWidth(dib);
		unsigned int height = FreeImage_GetHeight(dib);
		if ((width == 0) || (height == 0))
		{
			return false;
		}
		

		BYTE* bits = FreeImage_GetBits(dib);
		if (bits == 0)
		{
			return false;
		}

		BITMAPINFOHEADER* bitmap_header = FreeImage_GetInfoHeader(dib);
		if (bitmap_header->biBitCount == 32)
		{
			//RGBA
			//dib = FreeImage_ConvertTo32Bits(dib);
		}
		else
		{
			//RGB
			dib = FreeImage_ConvertTo32Bits(dib);
			if (!dib)
				return false;
			bits = FreeImage_GetBits(dib);
			_pixelSize = 4;
		}
		
		_width = width;
		_height = height;

		int bit_count = FreeImage_GetLine(dib)*_height;
		int bytespp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
		for (long index = 0; index < bit_count; index += bytespp)
		{
			unsigned char temp_color = bits[index];
			bits[index] = bits[index + 2];
			bits[index + 2] = temp_color;
		}

		_buffer = (unsigned char*)malloc(sizeof(char) * bit_count);
		memcpy(_buffer, bits, sizeof(char) * bit_count);
		_bufSize = bit_count;

		return true;
		
	}

	FIBITMAP* Image::rescale(FIBITMAP* dib, int width, int height)
	{
		//ûд
		return NULL;
	}

	FIBITMAP* Image::toBitMap()
	{
		FIBITMAP *dib = FreeImage_Allocate(_width, _height, BPP(), 0, 0, 0);

		int index = 0;
		int byte_spp = FreeImage_GetLine(dib) / FreeImage_GetWidth(dib);
		for (unsigned y = 0; y < FreeImage_GetHeight(dib); y++)
		{
			BYTE *bits = FreeImage_GetScanLine(dib, y);
			for (unsigned x = 0; x < FreeImage_GetWidth(dib); x++)
			{
				bits[FI_RGBA_RED] = _buffer[index++];
				bits[FI_RGBA_GREEN] = _buffer[index++];
				bits[FI_RGBA_BLUE] = _buffer[index++];
				if (BPP() == 4)
				{
					bits[FI_RGBA_ALPHA] = _buffer[index++];
				}
				bits += byte_spp;
			}
		}

		return dib;
	}
}