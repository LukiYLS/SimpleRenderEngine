#pragma once
#include <memory>
#include "Mesh.h"
namespace Core {

	class GeometryFactory {

	public:		
		static RenderObject::ptr MakeBox(int width, int height, int depth);
		static Mesh* MakeSphere(double radius, int widthSegments, int heightSegments);		
		//static Mesh* MakeCone();
		//static Mesh* MakeTube();
	};
}