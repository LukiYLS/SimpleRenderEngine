#pragma once
#include <string.h>
#include "Mesh.h"
#include "../Utils/BoundingSphere.h"
using namespace Utils;
namespace SRE {

	class Code
	{
	public:
		Code() {}
		~Code() {}
	public:
		bool operator>(const Code& right_) const;
		bool operator<(const Code& right_) const;
		bool operator==(const Code& geocode) {
			int nRes = memcmp(&mData, &geocode, sizeof(mData));
			if (nRes == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	public:

		union {
			struct {
				int x;
				int y;
				int level;
			};
			int mData[3];
		};
	};
	class TerrianTile
		:public RenderObject {
	public:
		TerrianTile();
		~TerrianTile();
		Mesh* createFromRandomHeght(int width, int height);
		Mesh* createMeshFromHeightmap(const char* fileName);
		void loadFromHeightMap(const char* fileName);
	protected:
		void generateVertex();
		void computerNormal();
		float getHeightValue(const unsigned char* data, unsigned char pixelSize);
	protected:
		int _width, _height;		
		Code _code;
		BoundingSphere _bounding_shpere;
		float *_data;
		float _blockScale;
		float _heightScale;
		//TerrianTile * _child[4];
	};
	
}