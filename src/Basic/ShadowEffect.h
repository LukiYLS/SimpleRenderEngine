#pragma once

#include "Effect.h"
#include "Entity.h"
#include "Light.h"
#include "ViewPort.h"
namespace Core {

	class ShadowEffect
		:public Effect{
	public:
		virtual bool init();
		virtual void  render(std::vector<Entity::ptr> entitys);

		void setLights(std::vector<Light::ptr> lights) { _lights = lights; }
		void setViewPort(ViewPort::ptr viewport);
	private:
		std::vector<Light::ptr> _lights;
		ViewPort::ptr _viewPort;
	};
}