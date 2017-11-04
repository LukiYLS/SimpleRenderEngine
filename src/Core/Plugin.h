#pragma once
#include <memory>
#include "Camera.h"
namespace Core {
	class Scene;
	class Plugin {

	public:
		typedef std::shared_ptr<Plugin> ptr;
		virtual void init() = 0;//加载shader，设置uniform，或者
		virtual void render(Camera* camera) = 0;
	};
}