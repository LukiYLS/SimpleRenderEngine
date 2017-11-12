#pragma once
#include "Vector3D.h"
namespace Math {
	class Plane3D {
	public:
		Plane3D();
		Plane3D(double distant_to_origin_, const Vector3D& normal_);
		virtual ~Plane3D();
	public:
		
		double  distance_to_point(const Vector3D& point) const;
		bool	project_vector(const Vector3D& origin_vector, Vector3D& out_vector);
		bool    intersect(const Vector3D& origin, const Vector3D& dir, Vector3D& intersect_point);
	public:
		double distance_to_origin;
		Vector3D normal;
	};
}