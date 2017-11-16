#pragma once
#include "../Math/Vector2D.h"
#include "Ray.h"
#include "Camera.h"
#include "Object.h"
#include "../Utils/AnyValue.h"
namespace Core {
	class RayCaster {

	public:
		RayCaster(Ray* ray) {};
		RayCaster(Ray* ray, double near, double far) {};

	public:
		void setFromCamera(const Vector2D& coords, Camera* camera);
		void intersectObject(Object* object, AnyValue& intersects, bool recursive);
	protected:
		Ray::ptr _ray;
		double _near;
		double _far;
	};
}