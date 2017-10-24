#pragma once
#include "FrameBuffer.h"
#include "Plugin.h"
#include "Light.h"
#include "Mesh.h"
#include "ViewPort.h"
namespace Core {

	class ShadowMapPlugin
		:public Plugin{		
	public:		
		ShadowMapPlugin(std::vector<Light*> lights, std::vector<Mesh*> meshs)
			:_lights(lights), _meshs(meshs) {}
		virtual void  render(Camera* camera);	
		FrameBuffer* getFB() { return _fb.get(); }
	private:
		std::vector<Light*> _lights;
		std::vector<Mesh*> _meshs;
		FrameBuffer::ptr _fb;
	};
}