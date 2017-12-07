#pragma once
#include <memory>
#include "RenderObject.h"
namespace SRE {

	class GeometryFactory {

	public:		
		static RenderObject::ptr MakeBox(int width, int height, int depth);
		static RenderObject::ptr MakeSphere(double radius, int widthSegments, int heightSegments);
		//static Mesh* MakeSphereOld(double radius, int widthSegments, int heightSegments);		
		//static Mesh* MakeCone();
		//static Mesh* MakeTube();
	};
}