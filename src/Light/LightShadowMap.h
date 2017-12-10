#pragma once

#include "Light.h"
#include "../Core/Mesh.h"
namespace SRE {

	class RenderSystem;
	class LightShadowMap {


	public:


		enum ShadowMapType {
			PCFShadowMap,
			PCFSoftShadowMap
		};

		typedef std::shared_ptr<ShadowMapType> ptr;

	public:

		void render(std::vector<Light::ptr> lights,std::vector<Mesh::ptr> objects);

	};
}