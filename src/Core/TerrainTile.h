#pragma once
#include <string.h>
#include "RenderObject.h"



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
		friend class Terrain;
		TerrainTile();
		/************************************************************************/
		/* 地形瓦片进行四叉树分割,xStart,yStart左下角起始点坐标*/
		/************************************************************************/
		TerrainTile(Terrain* terrain, TerrianTile* parent, unsigned int xStart, unsigned int yStart, unsigned int width, unsigned int height, Code code);
		~TerrainTile();

		enum Alignment
		{
			/// 地形置于XZ平面（也就是说高度值存在在Y坐标上）
			ALIGN_X_Z = 0,			
			ALIGN_X_Y = 1,			
			ALIGN_Y_Z = 2
		};

		void importHeightMap(float* heightMap);
		RenderObject* createFromRandomHeght(int width, int height);
		RenderObject* createMeshFromHeightmap(const char* fileName);

		size_t getSize()const { return _size; }

		void setWidth(unsigned int width) { _width = width; }
		unsigned int getWidth()const { return _width; }
		void setHeight(unsigned int  height) { _height = height; }
		unsigned int getHeight()const { return _height; }
		Code getCode()const { return _code; }


		
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