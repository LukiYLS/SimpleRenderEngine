#pragma once
#include "Scene.h"
#include "Win.h"
namespace Core {

	class SimpleRenderEngine {
	protected:
		static SimpleRenderEngine* instance;
	public:
		static SimpleRenderEngine* getSingleton();

	public:

		Scene::ptr craeteScene(const string& name);
		Scene::ptr createSceneFromXml(const char* fileName);

		void createWindow(int width, int height);
		void setRenderSystem();

		void getCurrentScene();

		void startRender();
	private:
		map<string, Scene::ptr> _scene_map;
		Win::ptr _win;
		Scene::ptr _scene;
	};
}