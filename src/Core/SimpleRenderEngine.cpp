#include "SimpleRenderEngine.h"

namespace Core {

	SimpleRenderEngine* SimpleRenderEngine::instance(0);
	SimpleRenderEngine* SimpleRenderEngine::getSingleton() {
		if (!instance)
			instance = new SimpleRenderEngine();
		return instance;
	}
	Scene::ptr SimpleRenderEngine::craeteScene(const string& name)
	{
		Scene::ptr scene = make_shared<Scene>();
		_scene = scene;
		_scene_map.insert(make_pair(name, scene));
		return scene;
	}
	void SimpleRenderEngine::createWindow(int width, int height)
	{
		_win = make_shared<Win>();
		_win->createWindow(width, height);
	}
	void SimpleRenderEngine::startRender()
	{
		while (!_win->isClose())
		{
			_win->processInput();
			_scene->render();
		}
	}
}