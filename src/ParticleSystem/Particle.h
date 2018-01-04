#pragma once

#include "../Math/Vector3D.h"

using Math::Vector3D;
namespace SRE {


	class Particle {

	public:

		enum Type {
			Render,
			Emitter
		};

		Vector3D mPosition;
		Vector3D mDirection;
		Vector3D mColor;

		float mLifeTime;
		float 
	};
}