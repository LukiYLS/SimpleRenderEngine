#pragma once
#include <glm\glm.hpp>
namespace Core {
	
	//两种pick的方法，还有问题？

	//framebuffer
	glm::vec4 GetColorByIndex(int index);
	int GetIndexByColor(int r, int g, int b);
	int GetPickedColorIndexUnderMouse(int x, int y);

	//raycast
	//void Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2);
	bool RaySphereCollision(glm::vec3 vSphereCenter, float fSphereRadius, glm::vec3 vA, glm::vec3 vB);

}