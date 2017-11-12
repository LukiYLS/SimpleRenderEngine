#pragma once
#include "../Math/Vector3D.h"
#include <vector>
using namespace Math;
namespace Utils {

	class BoundingBox {	

	public:
		typedef std::shared_ptr<BoundingBox> ptr;
		inline BoundingBox():_min(FLT_MAX, FLT_MAX, FLT_MAX),_max(-FLT_MAX,	-FLT_MAX, -FLT_MAX){}
		inline BoundingBox(const BoundingBox& bb) :	_min(bb._min), _max(bb._max){}
		inline BoundingBox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax) :
			_min(xmin, ymin, zmin),
			_max(xmax, ymax, zmax) {}		
		inline BoundingBox(const Vector3D& min, const Vector3D& max) :
			_min(min),
			_max(max){}			
		inline void init()
		{
			_min = Vector3D(FLT_MAX, FLT_MAX, FLT_MAX);
			_max = Vector3D(-FLT_MAX, -FLT_MAX, -FLT_MAX);
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
			_min = Vector3D(xmin, ymin, zmin);
			_max = Vector3D(xmax, ymax, zmax);
		}

		inline void set(const Vector3D& min, const Vector3D& max)
		{
			_min = min;
			_max = max;
		}
		void setFromPoints(std::vector<Vector3D> points);

		inline double& xMin() { return _min.x; }
		inline double xMin() const { return _min.x; }

		inline double& yMin() { return _min.y; }
		inline double yMin() const { return _min.y; }

		inline double& zMin() { return _min.z; }
		inline double zMin() const { return _min.z; }

		inline double& xMax() { return _max.x; }
		inline double xMax() const { return _max.x; }

		inline double& yMax() { return _max.y; }
		inline double yMax() const { return _max.y; }

		inline double& zMax() { return _max.z; }
		inline double zMax() const { return _max.z; }

		
		inline const Vector3D center() const
		{
			//return (_min + _max)/2;
		}
		
		inline double radius() const
		{
			return sqrt(radius2());
		}		
		inline double radius2() const
		{
			return 0.25*((_max - _min).length()*((_max - _min).length()));
		}
		inline const Vector3D corner(unsigned int pos) const
		{
			return Vector3D(pos & 1 ? _max.x : _min.x, pos & 2 ? _max.y : _min.y, pos & 4 ? _max.z : _min.z);
		}


	protected:
		Vector3D _min;
		Vector3D _max;
	};
}