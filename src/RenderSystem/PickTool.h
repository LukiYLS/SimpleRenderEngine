#pragma once
#include <glm\glm.hpp>
namespace RenderSystem {

	glm::vec4 GetColorByIndex(int index);
	int GetIndexByColor(int r, int g, int b);
	int GetPickedColorIndexUnderMouse();
}