#pragma once
#include <glm\glm.hpp>
namespace RenderSystem {

	//framebuffer
	glm::vec4 GetColorByIndex(int index);
	int GetIndexByColor(int r, int g, int b);
<<<<<<< HEAD
	int GetPickedColorIndexUnderMouse(int x, int y);
=======
	int GetPickedColorIndexUnderMouse();

	//raycast
	void Get3DRayUnderMouse(glm::vec3* v1, glm::vec3* v2);
	bool RaySphereCollision(glm::vec3 vSphereCenter, float fSphereRadius, glm::vec3 vA, glm::vec3 vB);
>>>>>>> 993253b9dd6c7fe656934158789fc70c6c9aea9a
}