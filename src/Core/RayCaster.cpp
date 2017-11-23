#include "RayCaster.h"
#include "../Math/MathHelper.h"

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
			Object::Children childs = object->getChildren();
			for (auto child : childs)
				intersectObject(child, intersects, true);
		}
	}
}