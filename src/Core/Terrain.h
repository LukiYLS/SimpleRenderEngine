#pragma once

namespace SRE {

	class Terrain {

	public:

		unsigned int getMaxLevel()const { return _maxLevel; }

	protected:

		unsigned int _maxLevel;

		size_t _size;

		TerrainTile* _root;

	};
}