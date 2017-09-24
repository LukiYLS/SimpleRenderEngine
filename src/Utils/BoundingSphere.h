#pragma once
#include <glm\glm.hpp>
namespace Utils {

	class BoundingSphere {
	public:
		BoundingSphere():_center(0.0, 0.0, 0.0), _radius(-1.0) {}
		BoundingSphere(glm::vec3 center, float radius):_center(center), _radius(radius){}

		inline bool operator == (const BoundingSphere& rhs) const { return _center == rhs._center && _radius == rhs._radius; }
		inline bool operator != (const BoundingSphere& rhs) const { return _center != rhs._center || _radius == rhs._radius; }

		inline void set(const glm::vec3& center, float radius)
		{
			_center = center;
			_radius = radius;
		}

		inline glm::vec3& center() { return _center; }		
		inline const glm::vec3& center() const { return _center; }
		
		inline float& radius() { return _radius; }		
		inline float radius() const { return _radius; }

		void expandBy(const glm::vec3& v);
		void expandRadiusBy(const glm::vec3& v);
		void expandBy(const BoundingSphere& sh);
		void expandRadiusBy(const BoundingSphere& sh);
	protected:
		glm::vec3 _center;
		float _radius;
	};
}