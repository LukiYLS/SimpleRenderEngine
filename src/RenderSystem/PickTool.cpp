
#include <glew\glew.h>
#include "PickTool.h"

#define RGB_WHITE (0xFF | (0xFF<<8) | (0xFF<<16))

namespace RenderSystem
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
}

