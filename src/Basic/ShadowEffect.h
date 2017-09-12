#pragma once

#include "Effect.h"
#include "Entity.h"
#include "Light.h"
namespace Basic {

	class ShadowEffect
		:public Effect{
	public:
		virtual bool init();
		virtual void  render(std::vector<Entity::ptr> entitys);

		void setLights(std::vector<Light::ptr> lights) { _lights = lights; }
	private:
		std::vector<Light::ptr> _lights;
	};
}