#pragma once

#include "Scene.h"
namespace Core {

	class SimpleRenderEngine {
	protected:
		static SimpleRenderEngine* instance;
	public:
		static SimpleRenderEngine* getSingleton();

	public:

		Scene::ptr craeteScene();
		Scene::ptr createSceneFromXml(const char* fileName);

		void createWindow(int width, int height);
		void setRenderSystem();


		void startRender();

	};
}