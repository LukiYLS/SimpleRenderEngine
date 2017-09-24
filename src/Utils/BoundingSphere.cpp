#include "BoundingSphere.h"

namespace Utils {

	void BoundingSphere::expandBy(const glm::vec3& v)
	{
		glm::vec3 toCenter = v - _center;
		float length = toCenter.length();
		if (length > _radius)
		{
			float halfLength = (length - _radius) / 2;
			_center += toCenter * (halfLength / length);
			_radius += halfLength;
		}		
	}
	void BoundingSphere::expandRadiusBy(const glm::vec3& v)
	{
		float toCenterLength = (v - _center).length();
		if (toCenterLength > _radius) _radius = toCenterLength;
	}
	void BoundingSphere::expandBy(const BoundingSphere& sh) {

		float distance = (_center - sh.center()).length();
		
		if (distance + sh.radius() <= _radius)//inside
		{
			return;
		}		
		if (distance + _radius <= sh.radius())//contain
		{
			_center = sh._center;
			_radius = sh._radius;
			return;
		}
	
		//新的sphere包含这两个，
		double new_radius = (_radius + distance + sh.radius()) * 0.5;
		double ratio = (new_radius - _radius) / distance;

		_center[0] += (sh.center()[0] - _center[0]) * ratio;
		_center[1] += (sh.center()[1] - _center[1]) * ratio;
		_center[2] += (sh.center()[2] - _center[2]) * ratio;

		_radius = new_radius;
	}
	void BoundingSphere::expandRadiusBy(const BoundingSphere& sh) {
		float distance = (sh._center - _center).length() + sh._radius;
		if (distance > _radius) _radius = distance;
	}
}