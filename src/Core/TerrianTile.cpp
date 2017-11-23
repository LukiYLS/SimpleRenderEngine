#include "TerrianTile.h"
#include <FreeImage.h>
namespace SRE {

	TerrianTile::TerrianTile()
	{
		

	}
	TerrianTile::~TerrianTile()
	{

	}
	void TerrianTile::createFromRandomHeght(int width, int height)
	{
		_width = width;
		_height = height;
		generateVertex();
		for (int i = 0; i < _width*_height; i++)
		{
			_vertices[i].position_y = (float)rand() / (float)RAND_MAX - 0.5f;
		}
	}
	void TerrianTile::loadFromHeightMap(const char* fileName)
	{
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		FIBITMAP *dib(0);

		fif = FreeImage_GetFileType(fileName, 0);

		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(fileName);

		if (fif == FIF_UNKNOWN)
			return;


		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, fileName);

		if (!dib)
			return;

		dib = FreeImage_ConvertTo24Bits(dib);

		//BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);

		_width = FreeImage_GetWidth(dib);
		_height = FreeImage_GetHeight(dib);

		if ((_width == 0) || (_height == 0))
			return;
		generateVertex();
		int byteIndex = 0;
		for (unsigned int i = 0; i < _height; i++)
		{
			for (unsigned int j = 0; j < _width; j++)
			{
				RGBQUAD color;
				FreeImage_GetPixelColor(dib, j, i, &color);
				_vertices[i*_width + j].position_y = color.rgbGreen * 256.0f;
			}
			
		}


	}
	void TerrianTile::generateVertex()
	{
		float halfHeight = (_height - 1) / 2;
		float halfWidth = (_width - 2) / 2;
		for (int i = 0; i < _height; i++)
		{
			for (int j = 0; j < _width; j++)
			{
				//间隔计算点坐标
				Vertex v(j - halfWidth, 0.0, halfHeight - i, 0, 1, 0, j / (_width - 1), 1 - i / (_height - 1));
				_vertices.push_back(v);
			}
		}
		for (int i = 0; i < _height - 1; i++)
		{
			for (int j = 0; j < _width - 1; j++)
			{
				//两个三角形索引
				_indices.push_back(i*_width + j);
				_indices.push_back(i*_width + j + 1);
				_indices.push_back((i + 1)*_width + j);

				_indices.push_back((i + 1)*_width + j);
				_indices.push_back(i*_width + j + 1);
				_indices.push_back((i + 1)*_width + j + 1);
			}
		}
	}
}