#pragma once
#include <memory>
#include "Camera.h"
namespace Core {
	class Scene;
	class Plugin {

	public:
		typedef std::shared_ptr<Plugin> ptr;
		virtual void render(Camera* camera) = 0;
	};
}