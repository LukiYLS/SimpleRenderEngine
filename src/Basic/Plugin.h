#pragma once
#include <memory>
namespace Core {
	class Scene;
	class Plugin {

	public:
		typedef std::shared_ptr<Plugin> ptr;
		virtual void render(Scene* scene) = 0;
	};
}