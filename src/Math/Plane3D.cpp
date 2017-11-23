#include "Plane3D.h"

#include "MathHelper.h"
namespace Math {

	Plane3D::Plane3D()
		:normal(0, 1, 0), distance_to_origin(0)
	{

	}

	Plane3D::Plane3D(double distant_to_origin_, const Vector3D& normal_)
		: distance_to_origin(distant_to_origin_), normal(normal_)
	{

	}

	Plane3D::~Plane3D()
	{

	}

	double Plane3D::distance_to_point(const Vector3D& point) const
	{
		return normal.dot(point) + distance_to_origin;
	}

	bool Plane3D::project_vector(const Vector3D& origin_vector, Vector3D& out_vector)
	{
		Vector3D tmp_vec(origin_vector);
		tmp_vec.normalize();
		if (fabs(tmp_vec.dot(normal)) > 1 - math_tolerance)//can not project correct,is this necessary??
		{
			return false;
		}
		else
		{
			out_vector = origin_vector - normal*(origin_vector*normal);
		}
		return true;
	}

	bool Plane3D::intersect(const Vector3D& origin, const Vector3D& dir, Vector3D& intersect_point)
	{
		double d = dir.dot(normal);
		double t = origin.dot(normal);
		if (fabs(d) < math_tolerance)
		{
			if (fabs(t + distance_to_origin) < math_tolerance)
			{
				intersect_point = origin;
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			t = (distance_to_origin - t) / d;
			intersect_point = origin + dir*t;
			return true;
		}
	}
}