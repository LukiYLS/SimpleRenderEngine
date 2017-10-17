#pragma once

#include "Plugin.h"
#include "Light.h"
#include "Mesh.h"
#include "ViewPort.h"
namespace Core {

	class ShadowMapPlugin
		:public Plugin{		
	public:		
		ShadowMapPlugin(std::vector<Light::ptr> lights, std::vector<Mesh::ptr> meshs)
			:_lights(lights), _meshs(meshs) {}
		virtual void  render();		
	private:
		std::vector<Light::ptr> _lights;
		std::vector<Mesh::ptr> _meshs;
	};
}