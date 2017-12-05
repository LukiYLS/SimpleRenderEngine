#pragma once

#include "Light.h"
#include "../Core/Mesh.h"
namespace SRE {

	class RenderSystem;
	class ShadowMap {

	public:


		void render(std::vector<Light::ptr> lights,std::vector<Mesh::ptr> objects, RenderSystem* rs);

	};
}