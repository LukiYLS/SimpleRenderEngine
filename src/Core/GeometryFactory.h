#pragma once
#include <memory>
#include "Mesh.h"
namespace Core {

	class GeometryFactory {

	public:		
		static Mesh* MakeBox(int width, int height, int depth);
		static Mesh* MakeSphere(int radius);
		static Mesh* MakeCone();
		static Mesh* MakeTube();
	};
}