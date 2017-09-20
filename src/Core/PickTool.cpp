
#include <glew\glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "PickTool.h"

#define RGB_WHITE (0xFF | (0xFF<<8) | (0xFF<<16))

namespace Core
{
	glm::vec4 GetColorByIndex(int index)
	{
		int r = index & 0xFF;
		int g = (index >> 8) & 0xFF;
		int b = (index >> 16) & 0xFF;

		return glm::vec4(float(r) / 255.0f, float(g) / 255.0f, float(b) / 255.0f, 1.0f);
	}

	int GetIndexByColor(int r, int g, int b)
	{

		return (r) | (g << 8) | (b << 16);
	}

	int GetPickedColorIndexUnderMouse(int x, int y)
	{

		unsigned char bArray[4];
		glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, bArray);
		int iResult = GetIndexByColor(bArray[0], bArray[1], bArray[2]);
		if (iResult == RGB_WHITE)return -1;
		return iResult;
	}

	/*void Get3DRayUnderMouse(int x, int y,int width, int height, glm::vec3* v1, glm::vec3* v2,Camera::ptr camera)
	{		

		glm::vec4 viewport = glm::vec4(0.0f, 0.0f, width, height);

		*v1 = glm::unProject(glm::vec3(float(x), float(y), 0.0f), camera->getViewMatrix(),camera->getProjectMatrix(), viewport);
		*v2 = glm::unProject(glm::vec3(float(x), float(y), 1.0f), camera->getViewMatrix(), camera->getProjectMatrix(), viewport);
	}*/

	bool RaySphereCollision(glm::vec3 vSphereCenter, float fSphereRadius, glm::vec3 vA, glm::vec3 vB)
	{
		// Create the vector from end point vA to center of sphere
		glm::vec3 vDirToSphere = vSphereCenter - vA;

		// Create a normalized direction vector from end point vA to end point vB
		glm::vec3 vLineDir = glm::normalize(vB - vA);

		// Find length of line segment
		float fLineLength = glm::distance(vA, vB);

		// Using the dot product, we project the vDirToSphere onto the vector vLineDir
		float t = glm::dot(vDirToSphere, vLineDir);

		glm::vec3 vClosestPoint;
		// If our projected distance from vA is less than or equal to 0, the closest point is vA
		if (t <= 0.0f)
			vClosestPoint = vA;
		// If our projected distance from vA is greater thatn line length, closest point is vB
		else if (t >= fLineLength)
			vClosestPoint = vB;
		// Otherwise calculate the point on the line using t and return it
		else
			vClosestPoint = vA + vLineDir*t;

		// Now just check if closest point is within radius of sphere
		return glm::distance(vSphereCenter, vClosestPoint) <= fSphereRadius;
	}
}

