#pragma once

#include "Plugin.h"
#include "Light.h"
#include "ViewPort.h"
namespace Core {

	class ShadowMapPlugin
		:public Plugin{
	public:		
		virtual void  render(Scene* scene);		
	private:
	};
}