#pragma once
#include <glm\glm.hpp>
namespace Utils {

	class BoundingBox {	

	public:
		inline BoundingBox():_min(FLT_MAX, FLT_MAX, FLT_MAX),_max(-FLT_MAX,	-FLT_MAX, -FLT_MAX){}
		inline BoundingBox(const BoundingBox& bb) :	_min(bb._min), _max(bb._max){}
		inline BoundingBox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) :
			_min(xmin, ymin, zmin),
			_max(xmax, ymax, zmax) {}		
		inline BoundingBox(const glm::vec3& min, const glm::vec3& max) :
			_min(min),
			_max(max){}			
		inline void init()
		{
			_min = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
			_max = glm::vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		}


		inline bool operator == (const BoundingBox& rhs) const { return _min == rhs._min && _max == rhs._max; }
		inline bool operator != (const BoundingBox& rhs) const { return _min != rhs._min || _max != rhs._max; }

		/** Returns true if the bounding box extents are valid, false otherwise. */
		inline bool valid() const
		{
			return _max.x >= _min.x && _max.y >= _min.y && _max.z >= _min.z;
		}

		
		inline void set(float xmin, float ymin, float zmin,
			float xmax, float ymax, float zmax)
		{
			_min = glm::vec3(xmin, ymin, zmin);
			_max = glm::vec3(xmax, ymax, zmax);
		}

		inline void set(const glm::vec3& min, const glm::vec3& max)
		{
			_min = min;
			_max = max;
		}


		inline float& xMin() { return _min.x; }
		inline float xMin() const { return _min.x; }

		inline float& yMin() { return _min.y; }
		inline float yMin() const { return _min.y; }

		inline float& zMin() { return _min.z; }
		inline float zMin() const { return _min.z; }

		inline float& xMax() { return _max.x; }
		inline float xMax() const { return _max.x; }

		inline float& yMax() { return _max.y; }
		inline float yMax() const { return _max.y; }

		inline float& zMax() { return _max.z; }
		inline float zMax() const { return _max.z; }

		
		inline const glm::vec3 center() const
		{
			//return (_min + _max)/2;
		}
		
		inline float radius() const
		{
			return sqrt(radius2());
		}		
		inline float radius2() const
		{
			return 0.25*((_max - _min).length()*((_max - _min).length()));
		}
		inline const glm::vec3 corner(unsigned int pos) const
		{
			return glm::vec3(pos & 1 ? _max.x : _min.x, pos & 2 ? _max.y : _min.y, pos & 4 ? _max.z : _min.z);
		}
	protected:
		glm::vec3 _min;
		glm::vec3 _max;
	};
}