#pragma once
#include <memory>
#include "Mesh.h"
namespace SRE {

	class GeometryFactory {

	public:		
		static Mesh* MakeBox(int width, int height, int depth);
		static Mesh* MakeSphere(double radius, int widthSegments, int heightSegments);
		static Mesh* MakeQuad(int width, int height);
		//static Mesh* MakeSphereOld(double radius, int widthSegments, int heightSegments);		
		//static Mesh* MakeCone();
		//static Mesh* MakeTube();
	};
}