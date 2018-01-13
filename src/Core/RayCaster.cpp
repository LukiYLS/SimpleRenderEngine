#include "RayCaster.h"
#include "../Math/MathHelper.h"
using namespace Math;
using namespace Utils;
namespace SRE {

	void RayCaster::setFromCamera(const Vector2D& coords, Camera* camera)
	{
		if (camera->type == Perspective)
		{
			Vector3D pos = camera->getPosition();
			Vector3D dir = MathHelper::unProject(camera->getViewMatrix(), camera->getProjectionMatrix(), Vector3D(coords.x, coords.y, 0.5)) - pos;
			_ray->set(pos, (dir - pos).normalize());
		}
		else
		{

		}
	}
	void RayCaster::intersectObject(Object* object, AnyValue& intersects, bool recursive)
	{
		object->raycast(this, intersects);

		if (recursive)
		{
			int count = object->getChildCount();
			for (int i = 0; i < count; i++)
			{
				Object::ptr child = object->getChild(i);
				intersectObject(child.get(), intersects, recursive);
			}
		}
	}
}