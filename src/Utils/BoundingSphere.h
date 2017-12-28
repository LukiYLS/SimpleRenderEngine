#pragma once
#include "../Math/Vector3D.h"
#include "../Math/Matrix4D.h"
#include <memory>
using namespace Math;
namespace Utils {

	class BoundingSphere {
	public:
		typedef std::shared_ptr<BoundingSphere> ptr;
		BoundingSphere():_center(0.0, 0.0, 0.0), _radius(-1.0) {}
		BoundingSphere(Vector3D center, float radius):_center(center), _radius(radius){}

		inline bool operator == (const BoundingSphere& rhs) const { return _center == rhs._center && _radius == rhs._radius; }
		inline bool operator != (const BoundingSphere& rhs) const { return _center != rhs._center || _radius == rhs._radius; }

		inline void set(const Vector3D& center, float radius)
		{
			_center = center;
			_radius = radius;
		}

		inline Vector3D& center() { return _center; }		
		inline const Vector3D& center() const { return _center; }
		
		inline float& radius() { return _radius; }		
		inline float radius() const { return _radius; }

		void expandByPoint(double x, double y, double z) { expandByPoint(Vector3D(x, y, z)); }
		void expandByPoint(const Vector3D& v);
		void expandRadiusBy(const Vector3D& v);
		void expandBy(const BoundingSphere& sh);
		void expandRadiusBy(const BoundingSphere& sh);

		void applyMatrix(const Matrix4D& matrix);
	protected:
		Vector3D _center;
		float _radius;
	};
}