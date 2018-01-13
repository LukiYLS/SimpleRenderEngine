#pragma once
#include "../Math/Vector3D.h"
#include "../Math/Vector4D.h"
namespace SRE {
	
	//两种pick的方法，还有问题？

	//framebuffer
	Math::Vector4D GetColorByIndex(int index);
	int GetIndexByColor(int r, int g, int b);
	int GetPickedColorIndexUnderMouse(int x, int y);

	//raycast
	//void Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2);
	bool RaySphereCollision(Math::Vector3D vSphereCenter, float fSphereRadius, Math::Vector3D vA, Math::Vector3D vB);

}