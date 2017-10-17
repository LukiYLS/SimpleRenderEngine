#pragma once
#include <string.h>
#include "../Utils/BoundingSphere.h"
using namespace Utils;
namespace Core {

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
	class TerrianTile {

	protected:

		Code _code;
		BoundingSphere _bounding_shpere;	
		TerrianTile * _child[4];
	};
	
}