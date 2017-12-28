#pragma once
#include <string.h>
#include "RenderObject.h"

#include "Terrain.h"
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
	/*
	地形瓦片是LOD，并且LOD的构建是基于四叉树构建，所以每个瓦片会有四个子节点
	*/
	class TerrainTile
		:public RenderObject {
	public:
		TerrainTile();
		TerrainTile(Terrain* terrain, TerrianTile* parent, unsigned int xStart, unsigned int yStart, size_t size, Code code);
		~TerrainTile();
		void importHeightMap(float* heightMap);
		RenderObject* createFromRandomHeght(int width, int height);
		RenderObject* createMeshFromHeightmap(const char* fileName);
		size_t getSize()const { return _size; }
		Code getCode()const{return _code}
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


		Code _code;//在整个地形中的坐标
		unsigned int _xStart, _yStart;
		size_t _size;
		TerrianTile* _parent;
		TerrianTile* _children[4];

		//TerrianTile * _child[4];
	};
	
}